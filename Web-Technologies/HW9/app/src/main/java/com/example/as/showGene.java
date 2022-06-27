package com.example.as;

import static android.content.ContentValues.TAG;
import com.squareup.picasso.Picasso;
import android.app.AlertDialog;
import android.app.Dialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.text.method.LinkMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.app.AppCompatDialogFragment;
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

import java.io.IOException;
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.util.ArrayList;

public class showGene extends AppCompatDialogFragment {
    gene g;

    public showGene(gene g) {
        this.g = g;
    }

    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState) {
        LayoutInflater inflater = getLayoutInflater();
        View dialoglayout = inflater.inflate(R.layout.gene_layout, null);
        AlertDialog.Builder builder = new AlertDialog.Builder(getActivity());
        builder.setView(dialoglayout);
        TextView name = dialoglayout.findViewById(R.id.gene_name);
        TextView dis = dialoglayout.findViewById(R.id.gene_desc);
        ImageView img = dialoglayout.findViewById((R.id.gene_image));
        name.setText(g.getGene_name());
        dis.setText(g.getGene_info());
        Log.d("t", g.getGene_name());
        if (g.getGene_img().equals("./assets/logo.png")) {
            img.setImageResource(R.drawable.logo);
        }

        else {
            Picasso.get().load(g.getGene_img()).error(R.drawable.logo).into(img);
            Log.d("t", "here");
                try {

                        //        img.setImageDrawable(drawableFromUrl(g.getGene_img()));
                    URL imageURL = new URL(g.getGene_img());
                    img.setImageBitmap(BitmapFactory.decodeStream(imageURL.openStream()));

                } catch (Throwable er) {
                    Log.d("error image", er.toString());
                    Log.d("url", g.getGene_img());
                    img.setImageResource(R.drawable.logo);
                }
            }
        return builder.create();
    }

    public static Drawable drawableFromUrl(String url) throws IOException {
        Bitmap x;
        HttpURLConnection connection = (HttpURLConnection) new URL(url).openConnection();
        connection.connect();
        InputStream input = connection.getInputStream();
        x = BitmapFactory.decodeStream(input);
        return new BitmapDrawable(Resources.getSystem(), x);
    }

}
