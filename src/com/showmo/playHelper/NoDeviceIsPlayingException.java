package com.showmo.playHelper;

public class NoDeviceIsPlayingException extends Exception{
    public NoDeviceIsPlayingException(String detailMessage) {
        super(detailMessage);
    }
}
