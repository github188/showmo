package com.showmo.util;

import java.util.concurrent.TimeUnit;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

import javax.mail.MessagingException;

import com.showmo.mail.MyMail;

import android.util.Log;

public class MailUtils {//"developer@puwell.com", "puwell123456"
	public static final String InternalEmailAddr="developer@puwell.com";
	public static final String InternalEmailAddrPsw="puwell123456";
	public static Lock lock=new ReentrantLock(); 
	public static boolean sendMail(String subject, String content,String sendAddr,String psw,String toAddr,String[] attachFilePathArr){
		
		MyMail m = new MyMail(sendAddr, psw);
        m.set_debuggable(false);
        String[] toArr = {toAddr};
        m.set_to(toArr);
        m.set_from(sendAddr);
        m.set_subject(subject);
        m.setBody(content);
        Log.e("send", "sendMail tryLock");
        try {
        	lock.lock();
		} catch (Exception e) {
			// TODO: handle exception
			e.printStackTrace();
			return false;
		}
        try {
        	if(attachFilePathArr!=null){
	        	for (int i = 0; i < attachFilePathArr.length; i++) {
	        		 m.addAttachment(attachFilePathArr[i]);
				}
        	}
            if(m.send()) {
                return true;
            } else {
                return false;
            }
        }
        catch (MessagingException e) {
            e.printStackTrace();
        }catch (Exception e) {
			// TODO: handle exception
        	 e.printStackTrace();
		}finally{
			Log.e("send", "sendMail unlock");
			lock.unlock();
		}
        return false;
	}
}
