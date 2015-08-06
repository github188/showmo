package com.showmo.util;

import java.io.UnsupportedEncodingException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.security.NoSuchProviderException;
import java.security.SecureRandom;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.KeyGenerator;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.SecretKey;
import javax.crypto.spec.SecretKeySpec;

/**
 * 加密工具类，对称加密AES
 * @author Terry
 *
 */
public class AESUtil {
	
	public static String KEY_AES = "01020304";
	
	 /** 
     * 加密 
     * @method encrypt 
     * @param content   需要加密的内容 
     * @param password  加密密码 
     * @return 
     * @throws  
     * @since v1.0 
     */  
    public static byte[] encrypt(String content, String password){  
        try {  
            KeyGenerator kgen = KeyGenerator.getInstance("AES");  
            SecureRandom sr;
			try {
				sr = SecureRandom.getInstance("SHA1PRNG","Crypto" );
				sr.setSeed(password.getBytes());
			} catch (NoSuchProviderException e) {
				e.printStackTrace();
				return null ;
			}
            kgen.init(128, sr);  
            SecretKey secretKey = kgen.generateKey();  
            byte[] enCodeFormat = secretKey.getEncoded();  
            SecretKeySpec key = new SecretKeySpec(enCodeFormat, "AES");  
            Cipher cipher = Cipher.getInstance("AES");// 创建密码器  
            
            byte[] byteContent = content.getBytes("utf-8");  
            cipher.init(Cipher.ENCRYPT_MODE, key);// 初始化  
            byte[] result = cipher.doFinal(byteContent);  
            return result; // 加密  
        } catch (NoSuchAlgorithmException e) {  
            e.printStackTrace();  
        }catch (NoSuchPaddingException e) {  
            e.printStackTrace();  
        }catch (UnsupportedEncodingException e) {  
            e.printStackTrace();  
        }catch (InvalidKeyException e) {  
            e.printStackTrace();  
        }catch (IllegalBlockSizeException e) {  
            e.printStackTrace();  
        }catch (BadPaddingException e) {  
            e.printStackTrace();  
        }  
        return null;  
    }  
      
    /** 
     * 解密 
     * @method decrypt 
     * @param content   待解密内容 
     * @param password  解密密钥 
     * @return 
     * @throws  
     * @since v1.0 
     */  
    public static byte[] decrypt(byte[] content, String password){  
        try {  
            
            KeyGenerator kgen = KeyGenerator.getInstance("AES");  
            SecureRandom sr;
			try {
				sr = SecureRandom.getInstance("SHA1PRNG","Crypto" );
				sr.setSeed(password.getBytes());
			} catch (NoSuchProviderException e) {
				e.printStackTrace();
				return null ;
			}
            
            kgen.init(128, sr);  
            SecretKey secretKey = kgen.generateKey();  
            byte[] enCodeFormat = secretKey.getEncoded();  
            SecretKeySpec key = new SecretKeySpec(enCodeFormat, "AES");  
            Cipher cipher = Cipher.getInstance("AES");// 创建密码器  
            cipher.init(Cipher.DECRYPT_MODE, key);// 初始化  
            byte[] result = cipher.doFinal(content);  
            return result; // 解密  
        } catch (NoSuchAlgorithmException e) {  
            e.printStackTrace();  
        }catch (NoSuchPaddingException e) {  
            e.printStackTrace();  
        }catch (InvalidKeyException e) {  
            e.printStackTrace();  
        }catch (IllegalBlockSizeException e) {  
            e.printStackTrace();  
        }catch (BadPaddingException e) {  
            e.printStackTrace();  
        }  
          
        return null;  
    }  
      
    /** 
     * 将二进制转换成16进制 
     * @method parseByte2HexStr 
     * @param buf 
     * @return 
     * @throws  
     * @since v1.0 
     */  
    public static String parseByte2HexStr(byte buf[]){  
        StringBuffer sb = new StringBuffer();  
        for(int i = 0; i < buf.length; i++){  
            String hex = Integer.toHexString(buf[i] & 0xFF);  
            if (hex.length() == 1) {  
                hex = '0' + hex;  
            }  
            sb.append(hex.toUpperCase());  
        }  
        return sb.toString();  
    }  
      
    /** 
     * 将16进制转换为二进制 
     * @method parseHexStr2Byte 
     * @param hexStr 
     * @return 
     * @throws  
     * @since v1.0 
     */  
    public static byte[] parseHexStr2Byte(String hexStr){  
        if(hexStr.length() < 1)  
            return null;  
        byte[] result = new byte[hexStr.length()/2];  
        for (int i = 0;i< hexStr.length()/2; i++) {  
        	String highString=hexStr.substring(i*2, i*2+1);
            int high = Integer.parseInt(highString, 16);  
            String lowString=hexStr.substring(i*2+1, i*2+2);
            int low = Integer.parseInt(lowString, 16);  
            result[i] = (byte) (high * 16 + low);  
        }  
        return result;  
    }  
	
}
