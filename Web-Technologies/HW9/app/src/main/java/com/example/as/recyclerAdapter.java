package com.example.as;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.RecyclerView;

import org.w3c.dom.Text;

import java.util.ArrayList;

public class recyclerAdapter extends RecyclerView.Adapter<recyclerAdapter.MyViewHolder> {
    private ArrayList<Artist> artistlist;
    private OnNoteListener monNoteListener;
    public recyclerAdapter(ArrayList<Artist> artistlist, OnNoteListener onNoteListener){
        this.artistlist = artistlist;
        this.monNoteListener = onNoteListener;
    }

    public class MyViewHolder extends RecyclerView.ViewHolder implements View.OnClickListener {
        private TextView aname;
        private TextView abirth;
        private TextView anation;
        OnNoteListener onNoteListener;
        public MyViewHolder(final View view, OnNoteListener onNoteListener){
            super(view);
            aname = view.findViewById(R.id.artistname);
            abirth = view.findViewById(R.id.artistbirth);
            anation = view.findViewById(R.id.artistnation);
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
    public recyclerAdapter.MyViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View itemview = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item, parent, false);
        return new MyViewHolder(itemview, monNoteListener);
    }

    @Override
    public void onBindViewHolder(@NonNull recyclerAdapter.MyViewHolder holder, int position) {
        String name = artistlist.get(position).getArtistname();
        String birth = artistlist.get(position).getArtistbirth();
        String nation = artistlist.get(position).getArtistnation();
        holder.aname.setText(name);
        holder.abirth.setText(birth);
        holder.anation.setText(nation);
    }

    @Override
    public int getItemCount() {
        return artistlist.size();
    }

    public interface OnNoteListener{
        void OnNoteClick(int position);
    }

}
