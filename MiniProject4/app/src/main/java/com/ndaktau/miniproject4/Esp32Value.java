package com.ndaktau.miniproject4;

public class Esp32Value {
    private String statusSys,statusServo;
    private float ldrVal;



    public Esp32Value(String statusSys, String statusServo, float ldrVal) {
        setStatusSys(statusSys);
        setStatusServo(statusServo);
        setLdrVal(ldrVal);

    }

    public String getStatusSys() {
        return statusSys;
    }

    public void setStatusSys(String statusSys) {
        this.statusSys = statusSys;
    }

    public String getStatusServo() {
        return statusServo;
    }

    public void setStatusServo(String statusServo) {
        this.statusServo = statusServo;
    }

    public float getLdrVal() {
        return ldrVal;
    }

    public void setLdrVal(float ldrVal) {
        this.ldrVal = ldrVal;
    }
}
