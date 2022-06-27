package com.example.as;

import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.text.method.ScrollingMovementMethod;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link information#newInstance} factory method to
 * create an instance of this fragment.
 */
public class information extends Fragment {
    public detailclass artist;
    public information(detailclass artist) {
        this.artist = artist;
    }

    public static information newInstance(detailclass artist) {
        information fragment = new information(artist);
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        View view = inflater.inflate(R.layout.fragment_information, container, false);
        TextView a = view.findViewById(R.id.info_name);
        a.setText(artist.getName());
        TextView b = view.findViewById(R.id.info_birth);
        b.setText(artist.getBirth());
        TextView c = view.findViewById(R.id.info_death);
        c.setText(artist.getDeath());
        TextView d = view.findViewById(R.id.info_nation);
        d.setText(artist.getNation());
        TextView e = view.findViewById(R.id.info_bio);
        e.setMovementMethod(new ScrollingMovementMethod());
        e.setText(artist.getBio());
        return view;
    }
}