#contoroldef=CrimsonVGA.ctl
#resourcefile=WTIIFM00
#label=F0List
#part={
#partname=F0List
#partfile=F0List.ptr
#@x=0
#@y=0
#@width=10
#@height=10
#callback FormConstruct 0 
#callback FormRedisp 0 
#callback FormSetUp 18 
void F0ListFormSetUp(APLFORMPTR AplForm, MESSAGE* Message)
{
  unsigned short val = 2;

  SetCtrlProperty(MemorySize, VALUE_PID, usValue, val,
		  AplForm->Control);

  if (zaurusScreenSize == ZAURUS_SCREEN_QVGA) {
    SetCtrlProperty(CloseBtn,   LEFT_X_PID, usValue, 304*4, AplForm->Control);
    
    SetCtrlProperty(MemorySize, LEFT_X_PID, usValue, 230*4, AplForm->Control);
    SetCtrlProperty(MemorySize, TOP_Y_PID,  usValue, 210*4, AplForm->Control);
    SetCtrlProperty(MemorySize, WIDTH_PID,  usValue, 96*4-1, AplForm->Control);
    SetCtrlProperty(MemorySize, HEIGHT_PID, usValue, 29*4-1, AplForm->Control);
    
    /* F0FileList, F1FileList ‚Í Program ‚©‚ç‚Í‰æ–ÊˆÊ’u‚ð•ÏX‚Å‚«‚È‚¢ ;_; */
  }
}
#callback FormEnd 37 
void F0ListFormEnd(APLFORMPTR AplForm, MESSAGE* Message)
{
  USHORT MemValue;
  int availMem;

  /** set memory from MemorySlider **/
  GetCtrlProperty(MemorySize, VALUE_PID, usValue, MemValue, 
		  AplForm->Control, USHORT);

  availMem = GetMaxAvailHeapSize();

  switch (MemValue) {
  case 1:
    if (availMem > 8192*1024)
     memorySize = ((4096+512)*1024);
    else if (availMem < 2048*1024)
     memorySize = ((512)*1024);
    else
     memorySize = ((2048+256)*1024);
    break;
  case 2:
  default:
    if (availMem > 8192*1024)
     memorySize = ((8192+4096+512)*1024);
    else if (availMem < 2048*1024)
     memorySize = ((1024+512)*1024);
    else
     memorySize = ((4096+512)*1024);
    break;
  }

  if(availMem <= memorySize)
    memorySize = availMem - 100*1024;



}
#callback FormDeActivate 0 
#callback FormActivate 0 
#callback FormEventFilter 0 
#callback FormMenuAtrSet 0 
#callback FormMenuSelect 0 
#callback FormKeyExec 0 
#callback FormError 0 
#callback FormOtherEvent 0 
#callback FormMenuFilter 0 
}
#part={
#partname=F0ListSqLogo
#partfile=F0ListSqLogo.ptr
#@x=0
#@y=0
#@width=16
#@height=16
#eventproc ButtonSelectID 0 
#eventproc ButtonPressID 0 
#eventproc ButtonPressStartID 0 
#eventproc ButtonStillSelectID 0 
}
#part={
#partname=F0FileList
#partfile=F0FileList.ptr
#@x=24
#@y=16
#@width=212
#@height=106
#eventproc FileSelectID 11 
void F0ListFileSelect(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* FLMsg)
{
  extern char shortImageName[DFNAME_LEN];
  extern char vmPath[DFNAME_LEN];

  strcpy(shortImageName, (const char*)FLMsg->FileSelect.FileName);
  strcpy(vmPath, "F0:");

  Message->CommonMsg.MessageID = SqueakScnID;
  AplForm->IsExit = true;
}
#callback FILE_CALL_BACK_PID 0 
}
#part={
#partname=F1FileList
#partfile=F1FileList.ptr
#@x=24
#@y=128
#@width=212
#@height=106
#eventproc FileSelectID 11 
void F1ListFileSelect(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* FLMsg)
{
  extern char shortImageName[DFNAME_LEN];
  extern char vmPath[DFNAME_LEN];

  strcpy(shortImageName, (const char*)FLMsg->FileSelect.FileName);
  strcpy(vmPath, "F1:");

  Message->CommonMsg.MessageID = SqueakScnID;
  AplForm->IsExit = true;
}
#callback FILE_CALL_BACK_PID 0 
}
#part={
#partname=CloseBtn
#partfile=CloseBtn.ptr
#@x=624
#@y=0
#@width=16
#@height=16
#eventproc ButtonSelectID 4 
void CloseBtnBtnSelect(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* BtnMsg)
{
  ioExit();
}
#eventproc ButtonPressID 0 
#eventproc ButtonPressStartID 0 
#eventproc ButtonStillSelectID 0 
}
#part={
#partname=MemorySize
#partfile=MemorySize.ptr
#@x=280
#@y=200
#@width=192
#@height=23
#eventproc SliderSetID 14 
void MemorySizeSliderSliderSet(APLFORMPTR AplForm, MESSAGE* Message, const MESSAGE* SldMsg)
{
  switch (SldMsg->SliderSet.Value) {
  case 1:
    memorySize = ((2048+256)*1024);
    break;
  case 2:
    memorySize = ((4096+512)*1024);
    break;
  default:
    memorySize = ((4096+512)*1024);
    break;
  }
}
}
