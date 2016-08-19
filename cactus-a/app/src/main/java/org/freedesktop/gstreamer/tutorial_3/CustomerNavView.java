package org.freedesktop.gstreamer.tutorial_3;



import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.RectF;
import android.os.Build;
import android.util.AttributeSet;
import android.view.View;

import org.freedesktop.gstreamer.tutorial_3.R;

/**
 * Created by tony on 16-8-17.
 */
public class CustomerNavView extends View{

    private int mPaintColr;
    private float mPaintWidth;
    private Paint mPaint;
    private RectF mBounds;
    private float width;
    private float height;
    private float radius;
    float largeLength;  //this is the length of large white line
    float smallLength;  //this is the length of small white line
    float navLinesOffset; //this is used to move all the nav lines upper or lower

    public CustomerNavView(Context context){
        super(context);
        init();
    }

    public CustomerNavView(Context context, AttributeSet attrs){
        super(context,attrs);

        TypedArray typedArray = context.getTheme().obtainStyledAttributes(attrs, R.styleable.CustomerNavView,0,0);

        try{
            mPaintColr = typedArray.getColor(R.styleable.CustomerNavView_paint_color,0xFFFFFFFF); //set paint color to white and not transparent
            mPaintWidth = typedArray.getDimension(R.styleable.CustomerNavView_paint_width,3); //3 is the default line width
        }finally {
            typedArray.recycle();
        }

        init();
    }

    public CustomerNavView(Context context,AttributeSet attrs,int defStyleAttr){
        super(context,attrs,defStyleAttr);

        init();
    }


    private void init(){
        mPaint = new Paint(Paint.ANTI_ALIAS_FLAG);
        mPaint.setStyle(Paint.Style.STROKE);
        mPaint.setStrokeWidth(mPaintWidth);
        mPaint.setColor(mPaintColr);

        navLinesOffset = 100;
    }

    @Override protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec){
        super.onMeasure(widthMeasureSpec,heightMeasureSpec);
    }

    @Override protected void onSizeChanged(int w,int h,int oldw,int oldh){
        super.onSizeChanged(w,h,oldw,oldh);

        mBounds = new RectF(getLeft(),getTop(),getRight(),getBottom());
        height = mBounds.bottom - mBounds.top;
        radius = 200;

        smallLength = 10; //this is the length of small line
        largeLength = 15; //this is the length of long line

    }

    @Override protected void onDraw(Canvas canvas){
        super.onDraw(canvas);
        canvas.drawColor(0x00000000);  //set view background transparent

        float start_x,start_y,end_x,end_y;
        for(int i=30;i<=60;++i){
            start_x= radius *(float)Math.cos(Math.PI/180 * i * 6);
            start_y= radius *(float)Math.sin(Math.PI/180 * i * 6);
            if(i%5==0){
                end_x = start_x+largeLength*(float)Math.cos(Math.PI / 180 * i * 6);
                end_y = start_y+largeLength*(float)Math.sin(Math.PI/180 * i * 6);
            }else{
                end_x = start_x+smallLength*(float)Math.cos(Math.PI/180 * i * 6);
                end_y = start_y+smallLength*(float)Math.sin(Math.PI/180 * i * 6);
            }
            start_x+=mBounds.centerX();
            end_x+=mBounds.centerX();
            start_y+=mBounds.centerY() + navLinesOffset;
            end_y+=mBounds.centerY() + navLinesOffset;
            canvas.drawLine(start_x, start_y, end_x, end_y, mPaint);
        }

    }

}
