
#include "h264def.h"
#include "get_bits.h"
#include <string.h>

static inline int get_ue_golomb_31(GetBitContext *gb){
    unsigned int buf;
	
    OPEN_READER(re, gb);
    UPDATE_CACHE(re, gb);
    buf=GET_CACHE(re, gb);
	
    buf >>= 32 - 9;
    LAST_SKIP_BITS(re, gb, ff_golomb_vlc_len[buf]);
    CLOSE_READER(re, gb);
	
    return ff_ue_golomb_vlc_code[buf];
}

static inline int av_log2_c(unsigned int v)
{
    int n = 0;
    if (v & 0xffff0000) {
        v >>= 16;
        n += 16;
    }
    if (v & 0xff00) {
        v >>= 8;
        n += 8;
    }
    n += ff_log2_tab[v];
	
    return n;
}

#define av_log2       av_log2_c

static inline int _get_ue_golomb(GetBitContext *gb){
    unsigned int buf;
    int log;
	
    OPEN_READER(re, gb);
    UPDATE_CACHE(re, gb);
    buf=GET_CACHE(re, gb);
	
    if(buf >= (1<<27)){
        buf >>= 32 - 9;
        LAST_SKIP_BITS(re, gb, ff_golomb_vlc_len[buf]);
        CLOSE_READER(re, gb);
		
        return ff_ue_golomb_vlc_code[buf];
    }else{
        log= 2*av_log2(buf) - 31;
        buf>>= log;
        buf--;
        LAST_SKIP_BITS(re, gb, 32 - log);
        CLOSE_READER(re, gb);
		
        return buf;
    }
}

static inline int _get_se_golomb(GetBitContext *gb){
    unsigned int buf;
    int log;
	
    OPEN_READER(re, gb);
    UPDATE_CACHE(re, gb);
    buf=GET_CACHE(re, gb);
	
    if(buf >= (1<<27)){
        buf >>= 32 - 9;
        LAST_SKIP_BITS(re, gb, ff_golomb_vlc_len[buf]);
        CLOSE_READER(re, gb);
		
        return ff_se_golomb_vlc_code[buf];
    }else{
        log= 2*av_log2(buf) - 31;
        buf>>= log;
		
        LAST_SKIP_BITS(re, gb, 32 - log);
        CLOSE_READER(re, gb);
		
        if(buf&1) buf= -(buf>>1);
        else      buf=  (buf>>1);
		
        return buf;
    }
}

static inline int get_ue(GetBitContext *s){
    int show= show_bits(s, 24);
    int pos= get_bits_count(s);
    int i= _get_ue_golomb(s);
    int len= get_bits_count(s) - pos;
    int bits= show>>(24-len);

    return i;
}
static inline int get_se(GetBitContext *s){
    int show= show_bits(s, 24);
    int pos= get_bits_count(s);
    int i= _get_se_golomb(s);
    int len= get_bits_count(s) - pos;
    int bits= show>>(24-len);

    return i;
}

#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))


// int ff_h264_decode_seq_parameter_set(H264Context *h)
// {
//     MpegEncContext * const s = &h->s;

static void decode_scaling_list(MpegEncContext *s, uint8_t *factors, int size,
                                const uint8_t *jvt_list, const uint8_t *fallback_list){
    int i, last = 8, next = 8;
    const uint8_t *scan = size == 16 ? zigzag_scan : ff_zigzag_direct;
    if(!get_bits1(&s->gb)) /* matrix not written, we use the predicted one */
        memcpy(factors, fallback_list, size*sizeof(uint8_t));
    else
    for(i=0;i<size;i++){
        if(next)
            next = (last + _get_se_golomb(&s->gb)) & 0xff;
        if(!i && !next){ /* matrix not written, we use the preset one */
            memcpy(factors, jvt_list, size*sizeof(uint8_t));
            break;
        }
        last = factors[scan[i]] = next ? next : last;
    }
}

static void decode_scaling_matrices(MpegEncContext *s, SPS *sps, PPS *pps, int is_sps,
                                   uint8_t (*scaling_matrix4)[16], uint8_t (*scaling_matrix8)[64]){
    int fallback_sps = !is_sps && sps->scaling_matrix_present;
    const uint8_t *fallback[4] = {
        fallback_sps ? sps->scaling_matrix4[0] : default_scaling4[0],
        fallback_sps ? sps->scaling_matrix4[3] : default_scaling4[1],
        fallback_sps ? sps->scaling_matrix8[0] : default_scaling8[0],
        fallback_sps ? sps->scaling_matrix8[1] : default_scaling8[1]
    };
    if(get_bits1(&s->gb)){
        sps->scaling_matrix_present |= is_sps;
        decode_scaling_list(s,scaling_matrix4[0],16,default_scaling4[0],fallback[0]); // Intra, Y
        decode_scaling_list(s,scaling_matrix4[1],16,default_scaling4[0],scaling_matrix4[0]); // Intra, Cr
        decode_scaling_list(s,scaling_matrix4[2],16,default_scaling4[0],scaling_matrix4[1]); // Intra, Cb
        decode_scaling_list(s,scaling_matrix4[3],16,default_scaling4[1],fallback[1]); // Inter, Y
        decode_scaling_list(s,scaling_matrix4[4],16,default_scaling4[1],scaling_matrix4[3]); // Inter, Cr
        decode_scaling_list(s,scaling_matrix4[5],16,default_scaling4[1],scaling_matrix4[4]); // Inter, Cb
        if(is_sps || pps->transform_8x8_mode){
            decode_scaling_list(s,scaling_matrix8[0],64,default_scaling8[0],fallback[2]);  // Intra, Y
            decode_scaling_list(s,scaling_matrix8[1],64,default_scaling8[1],fallback[3]);  // Inter, Y
        }
    }
}

static inline unsigned get_ue_golomb_long(GetBitContext *gb)
{
    unsigned buf, log;

    buf = show_bits_long(gb, 32);
    log = 31 - av_log2(buf);
    skip_bits_long(gb, log);

    return get_bits_long(gb, log + 1) - 1;
}

static inline int decode_hrd_parameters(MpegEncContext * s, SPS *sps){
    int cpb_count, i;
    cpb_count = get_ue_golomb_31(&s->gb) + 1;

	if(cpb_count > 32U){
//		av_log(h->s.avctx, AV_LOG_ERROR, "cpb_count %d invalid\n", cpb_count);
		return -1;
    }

    get_bits(&s->gb, 4); /* bit_rate_scale */
    get_bits(&s->gb, 4); /* cpb_size_scale */
    for(i=0; i<cpb_count; i++){
        get_ue_golomb_long(&s->gb); /* bit_rate_value_minus1 */
        get_ue_golomb_long(&s->gb); /* cpb_size_value_minus1 */
        get_bits1(&s->gb);     /* cbr_flag */
    }
    sps->initial_cpb_removal_delay_length = get_bits(&s->gb, 5) + 1;
    sps->cpb_removal_delay_length = get_bits(&s->gb, 5) + 1;
    sps->dpb_output_delay_length = get_bits(&s->gb, 5) + 1;
    sps->time_offset_length = get_bits(&s->gb, 5);
    sps->cpb_cnt = cpb_count;
    return 0;
}

static inline int decode_vui_parameters(MpegEncContext * s, SPS *sps){
    int aspect_ratio_info_present_flag;
    unsigned int aspect_ratio_idc;

    aspect_ratio_info_present_flag= get_bits1(&s->gb);

    if( aspect_ratio_info_present_flag ) {
        aspect_ratio_idc= get_bits(&s->gb, 8);
        if( aspect_ratio_idc == EXTENDED_SAR ) {
            sps->sar.num= get_bits(&s->gb, 16);
            sps->sar.den= get_bits(&s->gb, 16);
        }else if(aspect_ratio_idc < FF_ARRAY_ELEMS(pixel_aspect)){
            sps->sar=  pixel_aspect[aspect_ratio_idc];
        }else{
//			av_log(h->s.avctx, AV_LOG_ERROR, "illegal aspect ratio\n");
            return -1;
        }
    }else{
        sps->sar.num=
        sps->sar.den= 0;
    }
//            s->avctx->aspect_ratio= sar_width*s->width / (float)(s->height*sar_height);

    if(get_bits1(&s->gb)){      /* overscan_info_present_flag */
        get_bits1(&s->gb);      /* overscan_appropriate_flag */
    }

    enum AVColorSpace colorspace;
    sps->video_signal_type_present_flag = get_bits1(&s->gb);
    if(sps->video_signal_type_present_flag){
        get_bits(&s->gb, 3);    /* video_format */
        sps->full_range = get_bits1(&s->gb); /* video_full_range_flag */

        sps->colour_description_present_flag = get_bits1(&s->gb);
        if(sps->colour_description_present_flag){
            sps->color_primaries = (AVColorPrimaries)get_bits(&s->gb, 8); /* colour_primaries */
            sps->color_trc       = (AVColorTransferCharacteristic)get_bits(&s->gb, 8); /* transfer_characteristics */
            sps->colorspace      = (AVColorSpace)get_bits(&s->gb, 8); /* matrix_coefficients */
            if (sps->color_primaries >= AVCOL_PRI_NB)
                sps->color_primaries  = AVCOL_PRI_UNSPECIFIED;
            if (sps->color_trc >= AVCOL_TRC_NB)
                sps->color_trc  = AVCOL_TRC_UNSPECIFIED;
            if (sps->colorspace >= AVCOL_SPC_NB)
                sps->colorspace  = AVCOL_SPC_UNSPECIFIED;
        }
    }

    if(get_bits1(&s->gb)){      /* chroma_location_info_present_flag */
        AVChromaLocation chroma_sample_location = (AVChromaLocation)(_get_ue_golomb(&s->gb)+1);  /* chroma_sample_location_type_top_field */
        _get_ue_golomb(&s->gb);  /* chroma_sample_location_type_bottom_field */
    }

    sps->timing_info_present_flag = get_bits1(&s->gb);
    if(sps->timing_info_present_flag){
        sps->num_units_in_tick = get_bits_long(&s->gb, 32);
        sps->time_scale = get_bits_long(&s->gb, 32);
        if(!sps->num_units_in_tick || !sps->time_scale){
//			av_log(h->s.avctx, AV_LOG_ERROR, "time_scale/num_units_in_tick invalid or unsupported (%d/%d)\n", sps->time_scale, sps->num_units_in_tick);
			return -1;
        }
        sps->fixed_frame_rate_flag = get_bits1(&s->gb);
    }

    sps->nal_hrd_parameters_present_flag = get_bits1(&s->gb);
    if(sps->nal_hrd_parameters_present_flag)
        if(decode_hrd_parameters(s, sps) < 0)
            return -1;
    sps->vcl_hrd_parameters_present_flag = get_bits1(&s->gb);
    if(sps->vcl_hrd_parameters_present_flag)
        if(decode_hrd_parameters(s, sps) < 0)
            return -1;
    if(sps->nal_hrd_parameters_present_flag || sps->vcl_hrd_parameters_present_flag)
        get_bits1(&s->gb);     /* low_delay_hrd_flag */
    sps->pic_struct_present_flag = get_bits1(&s->gb);
    if(!get_bits_left(&s->gb))
        return 0;
    sps->bitstream_restriction_flag = get_bits1(&s->gb);
    if(sps->bitstream_restriction_flag){
        get_bits1(&s->gb);     /* motion_vectors_over_pic_boundaries_flag */
        _get_ue_golomb(&s->gb); /* max_bytes_per_pic_denom */
        _get_ue_golomb(&s->gb); /* max_bits_per_mb_denom */
        _get_ue_golomb(&s->gb); /* log2_max_mv_length_horizontal */
        _get_ue_golomb(&s->gb); /* log2_max_mv_length_vertical */
        sps->num_reorder_frames= _get_ue_golomb(&s->gb);
        _get_ue_golomb(&s->gb); /*max_dec_frame_buffering*/

        if(get_bits_left(&s->gb) < 0){
            sps->num_reorder_frames=0;
            sps->bitstream_restriction_flag= 0;
        }

//		if(sps->num_reorder_frames > 16U /*max_dec_frame_buffering || max_dec_frame_buffering > 16*/){
//			av_log(h->s.avctx, AV_LOG_ERROR, "illegal num_reorder_frames %d\n", sps->num_reorder_frames);
//			return -1;
//		}
    }
    if(get_bits_left(&s->gb) < 0){
//		av_log(h->s.avctx, AV_LOG_ERROR, "Overread VUI by %d bits\n", -get_bits_left(&s->gb));
		return -1;
    }

    return 0;
}

int ff_h264_decode_seq_parameter_set( MpegEncContext * s, SPS *sps )
{
    int profile_idc, level_idc, constraint_set_flags = 0;
    unsigned int sps_id;
    int i;
	
    profile_idc= get_bits(&s->gb, 8);
    constraint_set_flags |= get_bits1(&s->gb) << 0;   //constraint_set0_flag
    constraint_set_flags |= get_bits1(&s->gb) << 1;   //constraint_set1_flag
    constraint_set_flags |= get_bits1(&s->gb) << 2;   //constraint_set2_flag
    constraint_set_flags |= get_bits1(&s->gb) << 3;   //constraint_set3_flag
    get_bits(&s->gb, 4); // reserved
    level_idc= get_bits(&s->gb, 8);
    sps_id= get_ue_golomb_31(&s->gb);
	
    if(sps_id >= MAX_SPS_COUNT) {
//		av_log(h->s.avctx, AV_LOG_ERROR, "sps_id (%d) out of range\n", sps_id);
		goto fail;
    }

    if(sps == NULL)
       goto fail;
	
    sps->time_offset_length = 24;
    sps->profile_idc= profile_idc;
    sps->constraint_set_flags = constraint_set_flags;
    sps->level_idc= level_idc;
    sps->full_range = -1;
	
    memset(sps->scaling_matrix4, 16, sizeof(sps->scaling_matrix4));
    memset(sps->scaling_matrix8, 16, sizeof(sps->scaling_matrix8));
    sps->scaling_matrix_present = 0;
    sps->colorspace = AVCOL_SPC_UNSPECIFIED; //AVCOL_SPC_UNSPECIFIED
	
    if(sps->profile_idc >= 100){ //high profile
        sps->chroma_format_idc= get_ue_golomb_31(&s->gb);
        if (sps->chroma_format_idc > 3U) {
//			av_log(h->s.avctx, AV_LOG_ERROR, "chroma_format_idc %d is illegal\n", sps->chroma_format_idc);
			goto fail;
        }
        if(sps->chroma_format_idc == 3)
            sps->residual_color_transform_flag = get_bits1(&s->gb);
        sps->bit_depth_luma   = get_ue(&s->gb) + 8;
        sps->bit_depth_chroma = get_ue(&s->gb) + 8;
        if (sps->bit_depth_luma > 12U || sps->bit_depth_chroma > 12U) {
//			av_log(	h->s.avctx, AV_LOG_ERROR, "illegal bit depth value (%d, %d)\n",
//					sps->bit_depth_luma, sps->bit_depth_chroma);
			goto fail;
        }
        sps->transform_bypass = get_bits1(&s->gb);
	      decode_scaling_matrices(s, sps, NULL, 1, sps->scaling_matrix4, sps->scaling_matrix8);
    }else{
        sps->chroma_format_idc= 1;
        sps->bit_depth_luma   = 8;
        sps->bit_depth_chroma = 8;
    }
	
    sps->log2_max_frame_num= get_ue(&s->gb) + 4;//streameye 不加4
    sps->poc_type= get_ue_golomb_31(&s->gb);
	
    if(sps->poc_type == 0){ //FIXME #define
        sps->log2_max_poc_lsb= get_ue(&s->gb) + 4;//streameye 不加4
    } else if(sps->poc_type == 1){//FIXME #define
        sps->delta_pic_order_always_zero_flag= get_bits1(&s->gb);
        sps->offset_for_non_ref_pic= get_se(&s->gb);
        sps->offset_for_top_to_bottom_field= get_se(&s->gb);
        sps->poc_cycle_length                = get_ue(&s->gb);
		
        if((unsigned)sps->poc_cycle_length >= FF_ARRAY_ELEMS(sps->offset_for_ref_frame)){
//			av_log(h->s.avctx, AV_LOG_ERROR, "poc_cycle_length overflow %u\n", sps->poc_cycle_length);
			goto fail;
		}
		
        for(i=0; i<sps->poc_cycle_length; i++)
            sps->offset_for_ref_frame[i]= get_se(&s->gb);
    }else if(sps->poc_type != 2){
//		av_log(h->s.avctx, AV_LOG_ERROR, "illegal POC type %d\n", sps->poc_type);
		goto fail;
    }
	
	sps->ref_frame_count= get_ue_golomb_31(&s->gb);
	if(sps->ref_frame_count > MAX_PICTURE_COUNT-2 || sps->ref_frame_count > 16U){
//		av_log(h->s.avctx, AV_LOG_ERROR, "too many reference frames\n");
        goto fail;
    }
    sps->gaps_in_frame_num_allowed_flag= get_bits1(&s->gb);
    sps->mb_width = get_ue(&s->gb) + 1;//streameye 不加1
    sps->mb_height= get_ue(&s->gb) + 1;//streameye 不加1
//	if((unsigned)sps->mb_width >= INT_MAX/16 || (unsigned)sps->mb_height >= INT_MAX/16 ||
//		av_image_check_size(16*sps->mb_width, 16*sps->mb_height, 0, h->s.avctx)){
//		av_log(h->s.avctx, AV_LOG_ERROR, "mb_width/height overflow\n");
//		goto fail;
//	}
	
    sps->frame_mbs_only_flag= get_bits1(&s->gb);
    if(!sps->frame_mbs_only_flag)
        sps->mb_aff= get_bits1(&s->gb);
    else
        sps->mb_aff= 0;
	
    sps->direct_8x8_inference_flag= get_bits1(&s->gb);
	
#ifndef ALLOW_INTERLACE
//	if(sps->mb_aff)
//		av_log(h->s.avctx, AV_LOG_ERROR, "MBAFF support not included; enable it at compile-time.\n");
#endif
    sps->crop= get_bits1(&s->gb);
    if(sps->crop){
        int crop_vertical_limit   = sps->chroma_format_idc  & 2 ? 16 : 8;
        int crop_horizontal_limit = sps->chroma_format_idc == 3 ? 16 : 8;
        sps->crop_left  = get_ue(&s->gb);
        sps->crop_right = get_ue(&s->gb);
        sps->crop_top   = get_ue(&s->gb);
        sps->crop_bottom= get_ue(&s->gb);
        if(sps->crop_left || sps->crop_top){
//			av_log(h->s.avctx, AV_LOG_ERROR, "insane cropping not completely supported, this could look slightly wrong ... (left: %d, top: %d)\n", sps->crop_left, sps->crop_top);
        }
        if(sps->crop_right >= crop_horizontal_limit || sps->crop_bottom >= crop_vertical_limit){
//			av_log(h->s.avctx, AV_LOG_ERROR, "brainfart cropping not supported, cropping disabled (right: %d, bottom: %d)\n", sps->crop_right, sps->crop_bottom);
/* It is very unlikely that partial cropping will make anybody happy.
* Not cropping at all fixes for example playback of Sisvel 3D streams
* in applications supporting Sisvel 3D. */
			sps->crop_left  =
				sps->crop_right =
				sps->crop_top   =
				sps->crop_bottom= 0;
        }
    }else{
        sps->crop_left  =
			sps->crop_right =
			sps->crop_top   =
			sps->crop_bottom= 0;
	}
	
	sps->vui_parameters_present_flag= get_bits1(&s->gb);
	if( sps->vui_parameters_present_flag )
		if (decode_vui_parameters(s, sps) < 0)
			goto fail;
		
	if(!sps->sar.den)
		sps->sar.den= 1;		

	return 0;
fail:
	return -1;
}

// BOOL H264ParseSps( unsigned char *pbuff, unsigned long dwBufflen, SPS *pstSPS )
// {
// 	MpegEncContext test={0};
// 	long bit_size = 8 * dwBufflen;
// 	int buffer_size = (bit_size+7)>>3;
// 	int iRes;
// 
// 	if(!pstSPS) return FALSE;
// 
//     test.gb.buffer       = pbuff;
//     test.gb.size_in_bits = bit_size;
//     test.gb.size_in_bits_plus8 = bit_size + 8;
//     test.gb.buffer_end   = pbuff + buffer_size;
//     test.gb.index        = 0;
// 
// 	iRes = ff_h264_decode_seq_parameter_set( &test, pstSPS );
// 	if(iRes)
// 		return FALSE;
// 	return TRUE;
// }