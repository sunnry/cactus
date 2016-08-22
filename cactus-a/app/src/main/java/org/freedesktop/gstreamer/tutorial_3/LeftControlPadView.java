package org.freedesktop.gstreamer.tutorial_3;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by tony on 16-8-19.
 */
public class LeftControlPadView extends View {

    private Paint mPaint;

    public LeftControlPadView(Context context){
        super(context);
        init();
    }

    public LeftControlPadView(Context context, AttributeSet attrs){
        super(context,attrs);
        init();
    }

    public LeftControlPadView(Context context,AttributeSet attrs,int defStyleAttr){
        super(context,attrs,defStyleAttr);
        init();
    }

    private void init(){
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mPaint.setColor(0xAAFFFFFF);
    }


    @Override protected void onDraw(Canvas canvas){
        super.onDraw(canvas);
        canvas.drawColor(0x00000000);
        canvas.drawCircle(50,50,50,mPaint);
    }
}
