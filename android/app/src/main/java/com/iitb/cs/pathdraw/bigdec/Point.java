package com.iitb.cs.pathdraw.bigdec;

import java.math.BigDecimal;

public class Point {

    public BigDecimal x, y;

    public Point(float x, float y){
        this.x=new BigDecimal(x);
        this.y=new BigDecimal(y);
    }

    public Point(BigDecimal x, BigDecimal y){
        this.x = new BigDecimal(x.toPlainString());
        this.y = new BigDecimal(y.toPlainString());
    }

    public Point(Point P){
        this.x = new BigDecimal(P.x.toPlainString());
        this.y = new BigDecimal(P.y.toPlainString());
    }

    public static Point circumcenter(Point A, Point B, Point C){
        BigDecimal D=(A.x.multiply(B.y.subtract(C.y)).add(B.x.multiply(C.y.subtract(A.y))).add(C.x.multiply(A.y.subtract(B.y)))).multiply(new BigDecimal(2));

        BigDecimal x = (modsq(A).multiply(B.y.subtract(C.y)).add(modsq(B).multiply(C.y.subtract(A.y))).add(modsq(C).multiply(A.y.subtract(B.y))));
        BigDecimal y = (modsq(A).multiply(C.x.subtract(B.x)).add(modsq(B).multiply(A.x.subtract(C.x))).add(modsq(C).multiply(B.x.subtract(A.x))));

        return new Point(x.divide(D), y.divide(D));
    }


    public static BigDecimal modsq(Point A){
        return A.x.pow(2).add(A.y.pow(2));
    }
}
