package com.example.as;

import static android.content.ContentValues.TAG;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.NonNull;
import androidx.fragment.app.Fragment;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link artwork#newInstance} factory method to
 * create an instance of this fragment.
 */
public class artwork extends Fragment implements recyclerAdapterArtwork.ItemClickListerner{

    RecyclerView recyclerView;
    ArrayList<artworkclass> dataholder;
    public RequestQueue queue;
    String aid ;
    gene g;
    public artwork(String aid) {
        this.aid = aid;
    }

    public static artwork newInstance(String aid) {
        artwork fragment = new artwork(aid);
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        queue = Volley.newRequestQueue(getActivity().getApplicationContext());
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        // Inflate the layout for this fragment

        View view=inflater.inflate(R.layout.fragment_artwork, container, false);
        view.findViewById(R.id.loadingPanel111).setVisibility(View.VISIBLE);
        recyclerView = view.findViewById(R.id.artwork_show);
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(getContext());
        recyclerView.setLayoutManager(layoutManager);
        dataholder = new ArrayList<>();
        searchartwork(aid);
        view.findViewById(R.id.loadingPanel111).setVisibility(View.GONE);
        return view;
    }

    public void searchartwork(String sid) {
        String searchurl = "https://hw8-ea7aluo2ea-wl.a.run.app" + "/artworks/?artist_id=" + sid+"&size=10";
        Log.d("tag", searchurl);

        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET,
                searchurl, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        process_reponse(response);
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                });
        request.setRetryPolicy(new DefaultRetryPolicy(100000, 1, 1.0f));
        queue.add(request);
        //MySingleton.getInstance(ShowSearchActivity.this).addToRequestQueue(newsReq);

    }

    public void process_reponse(JSONObject res){
        Log.d(TAG, "onResponse called");
        try {
            JSONArray r = res.getJSONArray("result");

            for (int i = 0; i < r.length(); i++) {
                JSONObject tmp = r.getJSONObject(i);
                String imgurl = tmp.getString("image");
                String aid = tmp.getString("ID");
                String title = tmp.getString("title");
                Log.d(title, title);
                dataholder.add(new artworkclass(title, aid, imgurl));
            }
        } catch (JSONException newsE) {
            newsE.printStackTrace();
        }
        setAdapter();
        if (dataholder.size() < 1){
            dataholder.add(new artworkclass("No Result Found", "", "xx"));
        }

    }


    ItemTouchHelper.SimpleCallback itemtouch = new ItemTouchHelper.SimpleCallback(0, ItemTouchHelper.LEFT) {
        @Override
        public boolean onMove(@NonNull RecyclerView recyclerView, @NonNull RecyclerView.ViewHolder viewHolder, @NonNull RecyclerView.ViewHolder target) {
            return false;
        }

        @Override
        public void onSwiped(@NonNull RecyclerView.ViewHolder viewHolder, int direction) {
            dataholder.remove(viewHolder.getAdapterPosition());
            adapter.notifyDataSetChanged();
        }
    };

    recyclerAdapterArtwork adapter;
    private void setAdapter() {
        adapter = new recyclerAdapterArtwork(dataholder, this);
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(getContext());
        recyclerView.setLayoutManager((layoutManager));
        recyclerView.setItemAnimator(new DefaultItemAnimator());
        new ItemTouchHelper(itemtouch).attachToRecyclerView(recyclerView);
        recyclerView.setAdapter(adapter);
    }


    @Override
    public void onItemClick(artworkclass artworkc) {
        Log.d(TAG, "click artwork1");
        Log.d(TAG, artworkc.getArtwork_id());
        getgene(artworkc.getArtwork_id());


    }

    public void getgene(String sid) {
        String searchurl = "https://hw8-ea7aluo2ea-wl.a.run.app" + "/genes/?artwork_id=" + sid;
        Log.d("tag", searchurl);

        JsonObjectRequest request = new JsonObjectRequest(Request.Method.GET,
                searchurl, null,
                new Response.Listener<JSONObject>() {
                    @Override
                    public void onResponse(JSONObject response) {
                        process_reponse1(response);
                    }
                },
                new Response.ErrorListener() {
                    @Override
                    public void onErrorResponse(VolleyError error) {
                        error.printStackTrace();
                    }
                });
        request.setRetryPolicy(new DefaultRetryPolicy(100000, 1, 1.0f));
        queue.add(request);
    }

    public void process_reponse1(JSONObject res) {
        Log.d(TAG, "onResponse called");
        try {
            JSONObject tmp = res.getJSONObject("result");
            String imgurl = tmp.getString("image");
            String title = tmp.getString("name");
            String info = tmp.getString("description");
            Log.d(title, title);
            // gene_img, String gene_name, String gene_year, String gene_info
            g = new gene(imgurl, title, "", info);
            Log.d(TAG, g.getGene_name());
            showGene gene = new showGene(g);
            gene.show(getFragmentManager(), "Example");
        } catch (JSONException newsE) {
            new AlertDialog.Builder(getActivity())
                    .setTitle("No result found")
                    .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                        }
                    })
                    .show();
            newsE.printStackTrace();

        }

    }
}