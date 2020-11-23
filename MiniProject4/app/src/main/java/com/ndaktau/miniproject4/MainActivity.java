package com.ndaktau.miniproject4;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "com.ndaktau.smarthomeminiproject4.TAG";
    private TextView labelStatus,labelLdr,labelServo,status,ldr,servo;
    private Button btnOn;
    private FirebaseDatabase database;
    private DatabaseReference myRef ;

    public boolean isBtn() {
        return btn;
    }

    public void setBtn(boolean btn) {
        this.btn = btn;
    }

    boolean btn = false;


    public FirebaseDatabase getDatabase() {
        return database;
    }

    public void setDatabase() {
        this.database  = FirebaseDatabase.getInstance();
    }

    public DatabaseReference getMyRef() {
        return myRef;
    }

    public void setMyRef() {
        this.myRef = getDatabase().getReference("esp32");
    }


    public Button getBtnOn() {
        return btnOn;
    }

    public void setBtnOn() {
        this.btnOn = findViewById(R.id.btnSystem);
    }

    public TextView getLabelStatus() {
        return labelStatus;
    }

    public void setLabelStatus() {
        this.labelStatus = findViewById(R.id.labelStatus);
    }

    public TextView getLabelLdr() {
        return labelLdr;
    }

    public void setLabelLdr() {
        this.labelLdr = findViewById(R.id.labelLdr);
    }

    public TextView getLabelServo() {
        return labelServo;
    }

    public void setLabelServo() {
        this.labelServo = findViewById(R.id.labelStatusServo);
    }





    public TextView getStatus() {
        return status;
    }

    public void setStatus() {
        this.status = findViewById(R.id.status);
    }

    public TextView getLdr() {
        return ldr;
    }

    public void setLdr() {
        this.ldr = findViewById(R.id.cahaya);
    }

    public TextView getServo() {
        return servo;
    }

    public void setServo() {
        this.servo = findViewById(R.id.statusServo);
    }





    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        setDatabase();
        setMyRef();
        setBtnOn();
        setLabelLdr();

        setLabelServo();
        setLabelStatus();
        setLdr();

        setServo();
        setStatus();
        showData();
    }

    private void readData(){
        getMyRef().child("status").setValue("system is on");
        getMyRef().addValueEventListener(new ValueEventListener() {
            @SuppressLint({"LongLogTag", "SetTextI18n"})
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                Esp32Value esp32Value = new Esp32Value(snapshot.child("status").getValue(String.class),snapshot.child("servo").getValue(String.class),snapshot.child("ldr").getValue(Float.class));
                getStatus().setText(": "+esp32Value.getStatusSys());
                getServo().setText(": "+esp32Value.getStatusServo());
                getLdr().setText(": "+esp32Value.getLdrVal()+" LUX");


                Log.i(TAG, "onDataChange: "+esp32Value.getStatusSys());
                Log.i(TAG, "onDataChange: "+esp32Value.getStatusServo());
                Log.i(TAG, "onDataChange: "+esp32Value.getLdrVal());


            }

            @SuppressLint("LongLogTag")
            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Log.w(TAG, "loadPost:onCancelled", error.toException());
            }
        });
    }

    @SuppressLint("SetTextI18n")
    private void showData(){
        getBtnOn().setOnClickListener(v -> {
            if(!isBtn()){
                getBtnOn().setText("OFF");
                readData();
                setBtn(true);
            }
            else{
                getBtnOn().setText("ON");
                getMyRef().child("status").setValue("system is off");
//               getMyRef().child("ldr").setValue(10);
                getMyRef().child("servo").setValue("servo is off");
                setBtn(false);
            }
        });
    }
}
