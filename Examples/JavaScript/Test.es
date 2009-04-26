#javascript

v = Vector3(1,2,3);
v.x+=5;
v.y+=8;
Debug.Info(v.toString());
Debug.Info(v.length());

World.addSphere(Vector3(1,1,1),2);

Debug.ShowProgress("My progress:");
for ( i = 0; i < 2; i++) {
   Debug.SetProgress(i/100.0);
  Debug.Sleep(10);
}
Debug.HideProgress();

Debug.Info("This will be written to the console.");

Debug.Message("Completed");