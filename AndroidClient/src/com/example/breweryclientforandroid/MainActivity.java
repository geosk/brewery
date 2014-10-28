package com.example.breweryclientforandroid;

import android.os.Bundle;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.View;
import android.widget.TabHost;

public class MainActivity extends Activity {

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);

		TabHost tabs = (TabHost) findViewById(android.R.id.tabhost);

		tabs.setup();

		TabHost.TabSpec spec = tabs.newTabSpec("tag1");

		spec.setContent(R.id.tab1);
		spec.setIndicator("Main");
		tabs.addTab(spec);

		spec = tabs.newTabSpec("tag2");
		spec.setContent(R.id.tab2);
		spec.setIndicator("BlueTooth");
		tabs.addTab(spec);

		spec = tabs.newTabSpec("tag3");
		spec.setContent(R.id.tab3);
		spec.setIndicator("About");
		tabs.addTab(spec);

		tabs.setCurrentTab(0);	}

	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		// Inflate the menu; this adds items to the action bar if it is present.
		getMenuInflater().inflate(R.menu.main, menu);
		return true;
	}

	public void btnStartClicked(View view){
		
	}

	public void btnEditClicked(View view){
		Intent intent = new Intent(this,EditStep.class);
		 
		//start the EditStep Activity
		this.startActivity(intent);
	}

	public void btnAddClicked(View view){
		Intent intent = new Intent(this,EditStep.class);
		 
		//start the EditStep Activity
		this.startActivity(intent);
	}
	
	public void btnDelClicked(View view){
		
	}
}
