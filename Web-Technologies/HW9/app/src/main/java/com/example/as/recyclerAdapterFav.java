package com.example.as;

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

public class recyclerAdapterFav extends RecyclerView.Adapter<recyclerAdapterFav.MyViewHolder> {
    private ArrayList<Artist> artistlist;
    private OnNoteListener monNoteListener;
    public recyclerAdapterFav(ArrayList<Artist> artistlist, OnNoteListener onNoteListener){
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
    public recyclerAdapterFav.MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemview = LayoutInflater.from(parent.getContext()).inflate(R.layout.artistsearch_layout, parent, false);
        return new MyViewHolder(itemview, monNoteListener);
    }

    @Override
    public void onBindViewHolder(@NonNull recyclerAdapterFav.MyViewHolder holder, int position) {
        Animation animation = AnimationUtils.loadAnimation(holder.itemView.getContext(), android.R.anim.slide_in_left);
        String name = artistlist.get(position).getArtistname();
        holder.aname.setText(name);
        holder.imageView.setImageResource(R.drawable.a);
        holder.itemView.startAnimation(animation);
    }

    @Override
    public int getItemCount() {
        return artistlist.size();
    }

    public interface OnNoteListener{
        void OnNoteClick(int position);
    }

}
