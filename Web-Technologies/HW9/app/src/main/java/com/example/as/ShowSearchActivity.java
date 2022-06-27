package com.example.as;

import static android.content.ContentValues.TAG;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SearchView;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

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

import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.concurrent.atomic.AtomicInteger;

public class ShowSearchActivity extends AppCompatActivity implements recyclerAdapterSearch.OnNoteListener{
    public ArrayList<searchresultclass> artistlist;
    public RecyclerView recyclerView;
    public RequestQueue queue;
    String lsid = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main1);
        findViewById(R.id.loadingPanel).setVisibility(View.VISIBLE);
        Intent intent = getIntent();
        artistlist = new ArrayList<>();
        queue = Volley.newRequestQueue(getApplicationContext());
        String query = intent.getStringExtra("name");
        if (query != null){
            lsid = query;}

        setTitle(lsid);
        search(lsid);


    }

    public void search(String sid) {
        if (sid == null){
            SharedPreferences sp = getSharedPreferences("favourites", Context.MODE_PRIVATE);
            sid = sp.getString("last_search", "");
            setTitle(sid);
        }
        else{
            SharedPreferences sp = getSharedPreferences("favourites", Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = sp.edit();
            editor.putString("last_search", sid);
            editor.commit();
        }
        String searchurl = "https://hw8-ea7aluo2ea-wl.a.run.app" + "/search/?artist=" + sid;
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
                artistlist.add(new searchresultclass(title, aid, imgurl));
            }
        } catch (JSONException newsE) {
            newsE.printStackTrace();
        }
        setTheme(R.style.Theme_AS);
        //setContentView(R.layout.artistsearch_layout);
        TextView footer = findViewById(R.id.footer);
        footer.setMovementMethod(LinkMovementMethod.getInstance());
        if (artistlist.size() < 1){
            artistlist.add(new searchresultclass("No Result Found", "", "xx"));
        }
        recyclerView = findViewById(R.id.artistsearchlayout);
        setAdapter();
        findViewById(R.id.loadingPanel).setVisibility(View.GONE);
    }

    ItemTouchHelper.SimpleCallback itemtouch = new ItemTouchHelper.SimpleCallback(0, ItemTouchHelper.LEFT) {
        @Override
        public boolean onMove(@NonNull RecyclerView recyclerView, @NonNull RecyclerView.ViewHolder viewHolder, @NonNull RecyclerView.ViewHolder target) {
            return false;
        }

        @Override
        public void onSwiped(@NonNull RecyclerView.ViewHolder viewHolder, int direction) {
            artistlist.remove(viewHolder.getAdapterPosition());
            adapter.notifyDataSetChanged();
        }
    };

    recyclerAdapterSearch adapter;

    private void setAdapter() {
        adapter = new recyclerAdapterSearch(artistlist, this);
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(getApplicationContext());
        recyclerView.setLayoutManager((layoutManager));
        recyclerView.setItemAnimator(new DefaultItemAnimator());
        new ItemTouchHelper(itemtouch).attachToRecyclerView(recyclerView);
        recyclerView.setAdapter(adapter);
    }

    @Override
    public void OnNoteClick(int position) {
        Log.d(TAG, "clicked");
        Intent intent = new Intent(this, ShowDetailActivity.class );
        intent.putExtra("name", artistlist.get(position).getName());
        intent.putExtra("id", artistlist.get(position).getId());
        startActivity(intent);
    }
}
