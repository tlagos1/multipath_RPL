TIMEOUT(700000);
var waitForGDB = true;
while (true) {
  log.log(time + ":" + id + ":" + msg + "\n");
  YIELD();
  if(waitForGDB == true){
  if(time < -1){
    try
    {
      java.lang.Thread.sleep(2000);
    }
    catch (e)
    {
      /*
       * This will happen if the sleep is woken up - you might want to check
       * if enough time has passed and sleep again if not - depending on how
       * important the sleep time is to you.
       */
    }
    waitForGDB = false;
  }
  }
}