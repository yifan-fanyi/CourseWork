package com.example.as;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;

public class recyclerAdapterSearch extends RecyclerView.Adapter<recyclerAdapterSearch.MyViewHolder> {
    private ArrayList<searchresultclass> artistlist;
    private OnNoteListener monNoteListener;
    public recyclerAdapterSearch(ArrayList<searchresultclass> artistlist, OnNoteListener onNoteListener){
        this.artistlist = artistlist;
        this.monNoteListener = onNoteListener;
    }

    public class MyViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private TextView aname;
        private ImageView imageView;
        OnNoteListener onNoteListener;
        public MyViewHolder(final View view, OnNoteListener onNoteListener){
            super(view);
            aname = view.findViewById(R.id.search_artistname);
            imageView = view.findViewById(R.id.search_image);
            this.onNoteListener = onNoteListener;
            view.setOnClickListener(this);
        }

        @Override
        public void onClick(View view) {
            onNoteListener.OnNoteClick(getAdapterPosition());
        }
    }

    @NonNull
    @Override
    public recyclerAdapterSearch.MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemview = LayoutInflater.from(parent.getContext()).inflate(R.layout.artistsearch_layout, parent, false);
        return new MyViewHolder(itemview, monNoteListener);
    }

    @Override
    public void onBindViewHolder(@NonNull recyclerAdapterSearch.MyViewHolder holder, int position) {
        Animation animation = AnimationUtils.loadAnimation(holder.itemView.getContext(), android.R.anim.slide_in_left);
        String name = artistlist.get(position).getName();
        String imgurl = artistlist.get(position).getImg();
        holder.aname.setText(name);
        if (name.equals("No Result Found") == false) {
            if (imgurl.equals("./assets/logo.png")) {
                holder.imageView.setImageResource(R.drawable.logo);
            } else {
                Log.d("t", "here");
                try {
                    Thread thread = new Thread(new Runnable() {

                        @Override
                        public void run() {
                            try  {
                                holder.imageView.setImageDrawable(drawableFromUrl(imgurl));
                            } catch (Exception e) {
                                e.printStackTrace();
                            }
                        }
                    });

                    thread.start();

                } catch (Throwable er) {
                    Log.d("error image", er.toString());
                    Log.d("url", imgurl);
                    holder.imageView.setImageResource(R.drawable.logo);
                }
            }
        }
        holder.itemView.startAnimation(animation);
    }
    public static Drawable drawableFromUrl(String url) throws IOException {
        Bitmap x;
        HttpURLConnection connection = (HttpURLConnection) new URL(url).openConnection();
        connection.connect();
        InputStream input = connection.getInputStream();
        x = BitmapFactory.decodeStream(input);
        return new BitmapDrawable(Resources.getSystem(), x);
    }

    @Override
    public int getItemCount() {
        return artistlist.size();
    }

    public interface OnNoteListener{
        void OnNoteClick(int position);
    }

}
