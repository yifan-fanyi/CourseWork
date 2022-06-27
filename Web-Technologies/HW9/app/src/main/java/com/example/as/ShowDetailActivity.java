package com.example.as;
import static android.content.ContentValues.TAG;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;

import com.android.volley.DefaultRetryPolicy;
import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.JsonObjectRequest;
import com.android.volley.toolbox.Volley;
import com.example.as.databinding.ActivityMain1Binding;
import com.example.as.databinding.ActivityMainBinding;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.navigation.NavigationBarView;
import com.google.android.material.snackbar.Snackbar;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.Transaction;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.fragment.app.Fragment;
import androidx.fragment.app.FragmentTransaction;
import androidx.navigation.NavController;
import androidx.navigation.Navigation;
import androidx.navigation.ui.AppBarConfiguration;
import androidx.navigation.ui.NavigationUI;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Set;


public class ShowDetailActivity extends AppCompatActivity implements recyclerAdapterArtwork.ItemClickListerner{
    BottomNavigationView bottomNavigationView;
    String artistid;
    artwork art;
    information info;
    MenuItem fav;
    detailclass dt;
    public RequestQueue queue;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setTheme(R.style.Theme_AS);
        setContentView(R.layout.nav);
        findViewById(R.id.loadingPanel11).setVisibility(View.VISIBLE);
        Intent intent = getIntent();
        setTitle(intent.getStringExtra("name"));
        artistid = intent.getStringExtra("id");
        art = new artwork(artistid);

        queue = Volley.newRequestQueue(getApplicationContext());

        details(artistid);


    }

    public void details(String sid) {
        String searchurl = "https://hw8-ea7aluo2ea-wl.a.run.app" + "/details/?artist_id=" ;
        if (sid != null){
            searchurl += sid;
        }


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
        Log.d(TAG, "details onResponse called");
        try {
            JSONObject tmp = res.getJSONObject("result");
                String name = tmp.getString("name");
                String birth = tmp.getString("birthday");
                String death = tmp.getString("deathday");
                String nation = tmp.getString("nationality");
                String biography = tmp.getString("biography");
                //id, String birth, String death, String bio
            dt = new detailclass(name, nation, artistid, birth, death, biography);
                info = new information(dt);
        } catch (JSONException newsE) {
            newsE.printStackTrace();
        }
        TextView footer = findViewById(R.id.footer);
        footer.setMovementMethod(LinkMovementMethod.getInstance());

        bottomNavigationView = findViewById(R.id.topnav);
        //getSupportFragmentManager().beginTransaction().replace(R.id.fragment, info).commit();
        FragmentTransaction ft = getSupportFragmentManager().beginTransaction();// begin  FragmentTransaction
        ft.add(R.id.fragment, info);                                // add    Fragment
        ft.commit();
        bottomNavigationView.setOnItemSelectedListener(new NavigationBarView.OnItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                switch (item.getItemId()){
                    case R.id.information:
                        getSupportFragmentManager().beginTransaction().replace(R.id.fragment, info).commit();
                        return true;
                    case R.id.artwork:
                        getSupportFragmentManager().beginTransaction().replace(R.id.fragment, art).commit();
                        return true;
                }
                return false;
            }
        });
        findViewById(R.id.loadingPanel11).setVisibility(View.GONE);
    }

    @Override
    public void onItemClick(artworkclass artworkc) {
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu){
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_like, menu);
        fav = menu.findItem(R.id.add_favorite);
        Intent intent = getIntent();
        String artist_id = intent.getStringExtra("id");
        SharedPreferences sp = getApplicationContext().getSharedPreferences("favourites", MODE_PRIVATE);
        int count = sp.getInt("count", 0);
        for (int i=0; i < count; i++){
            if (artist_id.equals(sp.getString("id"+Integer.toString(i), "")) == true){
                fav.setIcon(R.drawable.ic_baseline_star_24);
            }
        }
        fav.setOnMenuItemClickListener(new MenuItem.OnMenuItemClickListener() {
            @Override
            public boolean onMenuItemClick(MenuItem menuItem) {
                Intent intent = getIntent();
                String artist_name = dt.getName();
                String artist_id = dt.getId();
                String artist_birth = dt.getBirth();
                String artist_nation = dt.getNation();
                SharedPreferences sp = getApplicationContext().getSharedPreferences("favourites", MODE_PRIVATE);
                int count = sp.getInt("count", 0);
                boolean flag = false;
                int position = 0;
                for (int i=0; i < count; i++){
                    if (artist_id.equals(sp.getString("id"+Integer.toString(i), "")) == true){
                        flag = true;
                        position = i;
                        break;
                    }
                }
                SharedPreferences.Editor editor = sp.edit();
                TextView tv = findViewById(R.id.addedtofav);
                tv.setVisibility(View.VISIBLE);

                if (flag == false){
                    editor.putString("name"+Integer.toString(count), artist_name);
                    editor.commit();
                    editor.putString("id"+Integer.toString(count), artist_id);
                    editor.commit();
                    editor.putString("birth"+Integer.toString(count), artist_birth);
                    editor.commit();
                    editor.putString("nation"+Integer.toString(count), artist_nation);
                    editor.commit();
                    editor.putInt("count", count+1);
                    editor.commit();
                    menuItem.setIcon(R.drawable.ic_baseline_star_24);
                    Log.d(TAG, "added");
                    tv.setText("Added to Favourite");
                    new AlertDialog.Builder(ShowDetailActivity.this)
                            .setTitle("Added to Favourite")
                            .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                }
                            })
                            .show();
                }
                else {
                    for (int i=position+1; i<count; i++){
                        String aid = sp.getString("id"+Integer.toString(i), "");
                        String aname = sp.getString("name"+Integer.toString(i), "");
                        String abirth = sp.getString("birth"+Integer.toString(i), "");
                        String anation = sp.getString("nation"+Integer.toString(i), "");
                        editor.putString("name"+Integer.toString(i-1), aname);
                        editor.commit();
                        editor.putString("id"+Integer.toString(i-1), aid);
                        editor.commit();
                        editor.putString("birth"+Integer.toString(i-1), abirth);
                        editor.commit();
                        editor.putString("nation"+Integer.toString(i-1), anation);
                        editor.commit();
                    }
                    editor.putInt("count", count-1);
                    editor.commit();
                    menuItem.setIcon(R.drawable.ic_baseline_star_outline_24);
                    tv.setText("Removed from Favourite");
                    Log.d(TAG, "removed");
                    new AlertDialog.Builder(ShowDetailActivity.this)
                            .setTitle("Removed from Favourite")
                            .setPositiveButton(android.R.string.yes, new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                }
                            })
                            .show();
                }
                tv.setVisibility(View.GONE);
                Log.d("fav click",TAG);
                return true;
            }

        });


        return true;
    }


}

