package com.showmo.base;

public class AlreadyLoginException extends Exception {
	public AlreadyLoginException(String detailMessage) {
        super(detailMessage);
    }
}
