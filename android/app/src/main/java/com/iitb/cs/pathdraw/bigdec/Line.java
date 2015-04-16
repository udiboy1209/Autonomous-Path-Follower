package com.iitb.cs.pathdraw.bigdec;


import java.math.BigDecimal;

public class Line {
    public BigDecimal m, c;

    public Line(Point A, Point B){
        m = A.y.subtract(B.y).divide(A.x.subtract(B.x));
        c = A.y.subtract(m.multiply(A.x));
    }

    public Line(BigDecimal m, BigDecimal c){
        this.m=new BigDecimal(m.toPlainString());
        this.c=new BigDecimal(c.toPlainString());
    }

    public Line(Point A, BigDecimal m){
        this.m=new BigDecimal(m.toPlainString());
        this.c = A.y.subtract(m.multiply(A.x));
    }

    public BigDecimal perpendicular(){
        return new BigDecimal(-1).divide(m);
    }

    public Point intersection(Line L){
        BigDecimal x = c.subtract(L.c).subtract(L.m.subtract(m));
        BigDecimal y = m.multiply(x).add(c);

        return new Point(x, y);
    }
}
