package com.example.as;

import static android.content.ContentValues.TAG;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.SearchView;
import androidx.recyclerview.widget.DefaultItemAnimator;
import androidx.recyclerview.widget.ItemTouchHelper;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Calendar;

public class MainActivity extends AppCompatActivity implements recyclerAdapter.OnNoteListener{

    SharedPreferences sp;
    private SharedPreferences.OnSharedPreferenceChangeListener listener;
    public ArrayList<Artist> artistlist=new ArrayList<>();
    public RecyclerView recyclerView;
    recyclerAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        setTheme(R.style.SplashTheme);
        super.onCreate(savedInstanceState);
        setTheme(R.style.Theme_AS);
        setContentView(R.layout.activity_main);
        Calendar calendar= Calendar.getInstance();
        String current_date = "   "+DateFormat.getDateInstance(DateFormat.MEDIUM).format(calendar.getTime());
        TextView textViewDate = findViewById(R.id.text_view_date);
        textViewDate.setText(current_date);
        TextView footer = findViewById(R.id.footer);
        footer.setMovementMethod(LinkMovementMethod.getInstance());

        sp = getSharedPreferences("favourites", Context.MODE_PRIVATE);
        listener = new SharedPreferences.OnSharedPreferenceChangeListener() {
            @Override
            public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
                // Refresh display
                refreshDisplay();
            }
        };

        // Register the listener on the SharedPreferences
        sp.registerOnSharedPreferenceChangeListener(listener);
        SharedPreferences.Editor editor = sp.edit();

        editor.commit();
        recyclerView = findViewById(R.id.listview);

        int count = sp.getInt("count", 0);
        for (int i = 0; i< count; i++){
            artistlist.add(new Artist(sp.getString("name"+Integer.toString(i),""),
                    sp.getString("id"+Integer.toString(i),""),
                    sp.getString("nation"+Integer.toString(i),""),
                    sp.getString("birth"+Integer.toString(i),"")));

        }
        setAdapter();
    }

    public void refreshDisplay() {
        recyclerView = findViewById(R.id.listview);
        artistlist.clear();
        int count = sp.getInt("count", 0);
        Log.d("coount", Integer.toString(count));
        Log.d("name", sp.getString("name"+Integer.toString(0),""));
        Log.d("name", sp.getString("nation"+Integer.toString(0),""));
        Log.d("name", sp.getString("birth"+Integer.toString(0),""));
        for (int i = 0; i< count; i++){
            artistlist.add(new Artist(sp.getString("name"+Integer.toString(i),""),
                    sp.getString("id"+Integer.toString(i),""),
                    sp.getString("nation"+Integer.toString(i),""),
                    sp.getString("birth"+Integer.toString(i),"")));

        }
        setAdapter();
        //adapter.notifyDataSetChanged();
        Log.d(TAG, "refresh");
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
            int count = sp.getInt("count",0);
            int position = viewHolder.getAdapterPosition();
            SharedPreferences.Editor editor = sp.edit();
            for (int i=position+1; i<count; i++){
                String aid = sp.getString("id"+Integer.toString(i), "");
                String aname = sp.getString("name"+Integer.toString(i), "");
                String abirth = sp.getString("birth"+Integer.toString(i), "");
                String anation = sp.getString("nation"+Integer.toString(i), "");
                editor.putString("name"+Integer.toString(i-1), aname);
                editor.putString("id"+Integer.toString(i-1), aid);
                editor.putString("birth"+Integer.toString(i-1), abirth);
                editor.putString("nation"+Integer.toString(i-1), anation);
            }
            editor.putInt("count", count-1);
            editor.commit();
        }
    };



    private void setAdapter() {
        adapter = new recyclerAdapter(artistlist, this);
        RecyclerView.LayoutManager layoutManager = new LinearLayoutManager(getApplicationContext());
        recyclerView.setLayoutManager((layoutManager));
        recyclerView.setItemAnimator(new DefaultItemAnimator());
        new ItemTouchHelper(itemtouch).attachToRecyclerView(recyclerView);
        recyclerView.setAdapter(adapter);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        MenuItem menuItem = menu.findItem(R.id.action_search);
        SearchView searchView = (SearchView) menuItem.getActionView();
        searchView.setQueryHint("Please enter artist name.");
        searchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                // search here

                Intent intent = new Intent(getApplicationContext(), ShowSearchActivity.class );
                intent.putExtra("name", query);
                startActivity(intent);
                //findViewById(R.id.loadingPanel).setVisibility(View.GONE);
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                return false;
            }
        });
        return super.onCreateOptionsMenu(menu);
    }


    @Override
    public void OnNoteClick(int position) {
        Log.d(TAG, "show detail clicked");

        Intent intent = new Intent(this, ShowDetailActivity.class );
        intent.putExtra("name", artistlist.get(position).getArtistname());
        intent.putExtra("id", artistlist.get(position).getArtistid());
        startActivity(intent);


    }
}

