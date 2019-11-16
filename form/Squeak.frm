#contoroldef=CrimsonVGA.ctl
#resourcefile=WTIIFM00
#label=Squeak
#part={
#partname=Squeak
#partfile=Squeak.ptr
#@x=0
#@y=0
#@width=10
#@height=10
#callback FormConstruct 0 
#callback FormRedisp 7 
void SqueakFormRedisp(APLFORMPTR AplForm, WINDOWHD WnHandle)
{
  FormDefRedisp(AplForm, WnHandle);
  if (squeakStarted) {
    fullDisplayUpdate();
  }
}
#callback FormSetUp 16 
void SqueakFormSetUp(APLFORMPTR AplForm, MESSAGE* Message)
{
  if (zaurusScreenSize == ZAURUS_SCREEN_QVGA) {
    SetCtrlProperty(Status, LEFT_X_PID, usValue, 40*4, AplForm->Control);
    SetCtrlProperty(Status, WIDTH_PID,  usValue, 130*4-1, AplForm->Control);
    SetCtrlProperty(Status, HEIGHT_PID, usValue, 16*4-1, AplForm->Control);
    
    /* SetCtrlProperty(KEYPAD00, Dummy_Y_PID, usValue, 195*4, AplForm->Control); */

    SetCtrlProperty(SqueakSqLogo, LEFT_X_PID,  usValue, 304*4, AplForm->Control);
    SetCtrlProperty(KeyState, LEFT_X_PID, usValue, 270*4, AplForm->Control);
    SetCtrlProperty(BlueButton, LEFT_X_PID, usValue, 250*4, AplForm->Control);
  }
  
  formSetUpTime = GetSystemTickCount();
}
#callback FormEnd 18 
void SqueakFormEnd(APLFORMPTR AplForm, MESSAGE* Message)
{
  MESSAGE msg;
  short check;


  GetCtrlProperty(KeyState, VALUE_PID, sValue, check,
		  AplForm->Control, short);

  if (check == CHECK_OFF) {
    EntryRedispFunc(AplForm->FormWnHandle, 0, NULL, NULL);
    msg.CommonMsg.MessageID = ToolCloseID;
    ControlCall(KEYPAD00, AplForm->Control, &msg);
  }

  ioExitBody();
  
}
#callback FormDeActivate 0 
#callback FormActivate 0 
#callback FormEventFilter 8 
void SqueakFormEventFilter(APLFORMPTR AplForm, MESSAGE* Message)
{
  if (Message->CommonMsg.MessageID == SystemTickID) {
    if (Message->SystemTick.Count - formSetUpTime > 0) {
      main(AplForm);
    }
  }
}
#callback FormMenuAtrSet 0 
#callback FormMenuSelect 0 
#callback FormKeyExec 0 
#callback FormError 0 
#callback FormOtherEvent 0 
#callback FormMenuFilter 0 
}
#part={
#partname=SqueakSqLogo
#partfile=SqueakSqLogo.ptr
#@x=624
#@y=0
#@width=16
#@height=16
#eventproc ButtonSelectID 0 
#eventproc ButtonPressID 0 
#eventproc ButtonPressStartID 0 
#eventproc ButtonStillSelectID 0 
}
#part={
#partname=KeyState
#partfile=KeyState.ptr
#@x=-32768
#@y=-32768
#@width=0
#@height=0
#eventproc ButtonSelectID 0 
#eventproc ButtonPressID 0 
#eventproc ButtonPressStartID 0 
#eventproc ButtonStillSelectID 0 
#subpart={
#partname=CHECKBTNONICON
#partfile=""
#@x=570
#@y=0
#@width=28
#@height=16
}
#subpart={
#partname=CHECKBTNOFFICON
#partfile=""
#@x=570
#@y=0
#@width=28
#@height=16
}
}
#part={
#partname=BlueButton
#partfile=BlueButton.ptr
#@x=-32768
#@y=-32768
#@width=0
#@height=0
#eventproc ButtonSelectID 0 
#eventproc ButtonPressID 0 
#eventproc ButtonPressStartID 0 
#eventproc ButtonStillSelectID 0 
#subpart={
#partname=CHECKBTNONICON
#partfile=""
#@x=0
#@y=0
#@width=9
#@height=9
}
#subpart={
#partname=CHECKBTNOFFICON
#partfile=""
#@x=540
#@y=4
#@width=9
#@height=9
}
}
#part={
#partname=KEYPAD00
#partfile=KEYPAD00.ptr
#@x=0
#@y=308
#@width=640
#@height=174
#eventproc ResultExistID 0 
#eventproc ResultNonID 0 
}
#part={
#partname=Status
#partfile=Status.ptr
#@x=200
#@y=0
#@width=220
#@height=16
}
