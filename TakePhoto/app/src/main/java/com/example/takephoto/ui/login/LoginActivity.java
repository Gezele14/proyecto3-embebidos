package com.example.takephoto.ui.login;

import android.app.Activity;

import androidx.lifecycle.Observer;
import androidx.lifecycle.ViewModelProviders;

import android.content.Intent;
import android.os.Bundle;

import androidx.annotation.Nullable;
import androidx.annotation.StringRes;
import androidx.appcompat.app.AppCompatActivity;

import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ProgressBar;
import android.widget.TextView;
import android.widget.Toast;

import com.example.takephoto.MainActivity;
import com.example.takephoto.R;
import com.loopj.android.http.RequestParams;
import com.loopj.android.http.SyncHttpClient;
import com.loopj.android.http.TextHttpResponseHandler;

import java.io.File;

public class LoginActivity extends AppCompatActivity {

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_login);

        final EditText urlEditText = findViewById(R.id.url);
        final Button loginButton = findViewById(R.id.login);
        findViewById(R.id.loadingPanelSignIn).setVisibility(View.GONE);
        //final String urlValue = urlEditText.getText().toString();


        loginButton.setOnClickListener(new View.OnClickListener(){
            public void onClick(View v){
                findViewById(R.id.loadingPanelSignIn).setVisibility(View.VISIBLE);
                final String urlValue = urlEditText.getText().toString();
                final String urlValue1 = urlValue + "/upload";

                Thread t = new Thread(){
                    public void run(){
                        try {
                            SyncHttpClient client = new SyncHttpClient();
                            String get_url = urlValue + "/api/hello";

                            client.get(get_url, new TextHttpResponseHandler() {
                                @Override
                                public void onFailure(int statusCode, cz.msebera.android.httpclient.Header[] headers, String responseString, Throwable throwable) {
                                    findViewById(R.id.loadingPanelSignIn).setVisibility(View.INVISIBLE);
                                    showToast("Error HTTP");
                                }

                                @Override
                                public void onSuccess(int statusCode, cz.msebera.android.httpclient.Header[] headers, String responseString) {
                                    Intent intent = new Intent(LoginActivity.this, MainActivity.class);
                                    intent.putExtra("message", urlValue1);
                                    startActivity(intent);
                                    //startActivity(new Intent(LoginActivity.this, MainActivity.class));
                                }
                            });


                        } catch (Exception e){
                            Log.e("log_tag", "Error in http connection " + e.toString());
                            findViewById(R.id.loadingPanelSignIn).setVisibility(View.INVISIBLE);
                            showToast("Error HTTP");
                        }
                    }
                };
                t.start();
            }
        });
    }

    public void showToast(final String toast)
    {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                Toast.makeText(LoginActivity.this, toast, Toast.LENGTH_SHORT).show();
            }
        });
    }

}