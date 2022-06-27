package com.example.as;

import static com.example.as.recyclerAdapterSearch.drawableFromUrl;

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

import java.util.ArrayList;

public class recyclerAdapterArtwork extends RecyclerView.Adapter<recyclerAdapterArtwork.MyViewHolder> {
    private ArrayList<artworkclass> artworklist;
    private ItemClickListerner itemClickListerner;
    public recyclerAdapterArtwork(ArrayList<artworkclass> artworklist, ItemClickListerner itemClickListerner){
        this.artworklist = artworklist;
        this.itemClickListerner = itemClickListerner;
    }

    public class MyViewHolder extends RecyclerView.ViewHolder {
        private TextView aname;
        private ImageView imageView;
        public MyViewHolder(final View view){
            super(view);
            aname = view.findViewById(R.id.artwork_name);
            imageView = view.findViewById(R.id.artwork_image);
        }
    }

    @NonNull
    @Override
    public recyclerAdapterArtwork.MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemview = LayoutInflater.from(parent.getContext()).inflate(R.layout.artwork_layout, parent, false);
        return new MyViewHolder(itemview);
    }

    @Override
    public void onBindViewHolder(@NonNull recyclerAdapterArtwork.MyViewHolder holder, int position) {
        Animation animation = AnimationUtils.loadAnimation(holder.itemView.getContext(), android.R.anim.slide_in_left);
        String name = artworklist.get(position).getArtwork_name();
        holder.aname.setText(name);
        String imgurl = artworklist.get(position).getArtwork_image();
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
                    holder.imageView.setImageResource(R.drawable.logo);
                }
            }
        }
        holder.itemView.startAnimation(animation);
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                itemClickListerner.onItemClick(artworklist.get(position));
            }
        });
    }

    @Override
    public int getItemCount() {
        return artworklist.size();
    }


    public interface ItemClickListerner{
        void onItemClick(artworkclass artworkclass);
        //void onItemClick(artworkclass artworkclass);
    }

}
