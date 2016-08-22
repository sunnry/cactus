package org.freedesktop.gstreamer.tutorial_3;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.View;

/**
 * Created by tony on 16-8-19.
 */
public class LeftControlBaseView extends View {

    private Paint mPaint;

    public LeftControlBaseView(Context context){
        super(context);
        init();
    }

    public LeftControlBaseView(Context context, AttributeSet attrs){
        super(context,attrs);
        init();
    }

    public LeftControlBaseView(Context context,AttributeSet attrs,int defStyleAttr){
        super(context,attrs,defStyleAttr);
        init();
    }

    private void init(){
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mPaint.setColor(0x33FFFFFF);
    }

    @Override protected void onDraw(Canvas canvas){
        super.onDraw(canvas);
        canvas.drawColor(0x00FFFFFF); //when debugging, you can set canvas not transparent and see layout
        canvas.drawCircle(200,200,150,mPaint);
    }
}

