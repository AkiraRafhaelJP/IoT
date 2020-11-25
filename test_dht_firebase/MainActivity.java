package com.example.iot_mp_4_71180284;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;

import android.widget.CompoundButton;
import android.widget.TextView;
import android.widget.ToggleButton;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

public class MainActivity extends AppCompatActivity {

    ToggleButton buttonSuhu;

    TextView status;
    TextView suhu;

    String valueStatus;
    String valueSuhu;

    DatabaseReference dref;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        buttonSuhu = findViewById(R.id.toggleButton);

        status = findViewById(R.id.txtView_status);
        suhu = findViewById(R.id.txtView_suhu);

        dref = FirebaseDatabase.getInstance().getReference();
        dref.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot dataSnapshot) {
                valueStatus = dataSnapshot.child("Node1/status").getValue().toString();
                status.setText(valueStatus);

                valueSuhu = dataSnapshot.child("Node1/suhu").getValue().toString();
                suhu.setText(valueSuhu);
            }

            @Override
            public void onCancelled(@NonNull DatabaseError databaseError) {

            }
        });

        buttonSuhu.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    DatabaseReference suhuRef = FirebaseDatabase.getInstance().getReference("Node1/suhuToggled");
                    suhuRef.setValue(1);
                }
                else
                {
                    DatabaseReference suhuRef = FirebaseDatabase.getInstance().getReference("Node1/suhuToggled");
                    suhuRef.setValue(0);
                }
            }
        });
    }
}