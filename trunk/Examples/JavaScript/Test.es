#javascript

Debug.ShowProgress("My progress:");
for ( i = 0; i < 100; i++) {
   Debug.SetProgress(i/100.0);
  Debug.Sleep(10);
}
Debug.HideProgress();

Debug.Info("This will be written to the console.");

Debug.Message("Completed");