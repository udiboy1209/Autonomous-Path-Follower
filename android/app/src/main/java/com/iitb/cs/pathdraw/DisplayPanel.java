package com.iitb.cs.pathdraw;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.PointF;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.util.ArrayList;
import java.util.List;

public class DisplayPanel extends SurfaceView implements SurfaceHolder.Callback {
    DisplayThread thread;
    List<PointF> pts = new ArrayList<>();
    List<PointF> centres = new ArrayList<>();

    int updates_skipped=0, MAX_UPDATE_SKIPS=2;

    boolean plot=true;

    Paint RED=new Paint(), BLACK=new Paint(), ARC=new Paint();

    public DisplayPanel(Context context, AttributeSet attrs) {
        super(context, attrs);
        getHolder().addCallback(this);
        setFocusable(true);
        setWillNotDraw(false);

        thread = new DisplayThread(getHolder(),this);

        RED.setColor(0xFFFF0000);
        BLACK.setColor(0xFF000000);

        ARC.setStyle(Paint.Style.STROKE);
        ARC.setColor(0xFF000000);
        ARC.setStrokeWidth(1);
    }

    @Override
    public void surfaceChanged(SurfaceHolder arg0, int arg1, int arg2, int arg3) {
    }

    @Override
    public void surfaceCreated(SurfaceHolder arg0) {
        thread.setRunning(true);
        thread.start();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder arg0) {
        Log.d("Panel", "Destroying surface");

        thread.setRunning(false);
        try {
            thread.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }

    public void update(){
        updates_skipped++;

        if(updates_skipped==MAX_UPDATE_SKIPS) {
            updates_skipped = 0;
            plot = true;
        }

        if(pts.size()>2) {
            centres.clear();
            centres.add(circumcenter(pts.get(0), pts.get(1), pts.get(2)));
            centres.add(centres.get(0));

            for (int i = 2; i < pts.size()-1; i++) {
                PointF p1=pts.get(i), p2=pts.get(i+1), c=centres.get(i-1);
                float A=(p1.y-c.y)/(p1.x-c.x),
                      B=(c.y*p1.x-c.x*p1.y)/(p1.x-c.x),
                      C=(p1.x-p2.x)/(p2.y-p1.y),
                      D=(modsq(p2)-modsq(p1))/(2*(p2.y-p1.y));

                centres.add(new PointF((D-B)/(A-C), (D*A-B*C)/(A-C)));
            }
        }
    }

    public void render(Canvas canvas){
        canvas.drawARGB(255,255,255,255);

        for(int i=0; i<centres.size(); i++){
            PointF p1= pts.get(i), p2=pts.get(i+1), c=centres.get(i);
            float radius = distance(p1,c),
                  startAngle=(float)(Math.atan((p1.y-c.y)/(p1.x-c.x))*180/Math.PI),
                  endAngle=(float)(Math.atan((p2.y-c.y)/(p2.x-c.x))*180/Math.PI);

            //canvas.drawCircle(c.x, c.y, radius, ARC);

            canvas.drawCircle(c.x, c.y, radius, ARC);
            canvas.drawCircle(p1.x, p1.y, 2, RED);
            canvas.drawCircle(c.x, c.y, 2, BLACK);
        }
    }

    public PointF midpoint(PointF A, PointF B){
        return new PointF((A.x+B.x)/2, (A.y+B.y)/2);
    }

    public PointF circumcenter(PointF A, PointF B, PointF C){
        float D=2*(A.x*(B.y-C.y)+B.x*(C.y-A.y)+C.x*(A.y-B.y));

        float x = (modsq(A)*(B.y-C.y)+modsq(B)*(C.y-A.y)+modsq(C)*(A.y-B.y));
        float y = (modsq(A)*(C.x-B.x)+modsq(B)*(A.x-C.x)+modsq(C)*(B.x-A.x));

        return new PointF(x/D,y/D);
    }

    public float modsq(PointF A){
        return A.x*A.x + A.y*A.y;
    }

    public float distance(PointF a, PointF b){
        return (float)Math.sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    }

    @Override
    public boolean onTouchEvent(MotionEvent me){
        switch (me.getAction()){
            case MotionEvent.ACTION_MOVE:
                if(plot){
                    pts.add(new PointF(me.getX(), me.getY()));
                    plot=false;
                    Log.d("PathDisplay", "plot");
                }
                Log.d("PathDisplay", "move");
                break;
            case MotionEvent.ACTION_DOWN:
                pts.clear();
                centres.clear();
                plot=true;
                Log.d("PathDisplay", "down");
                break;
        }
        return true;
    }
}
