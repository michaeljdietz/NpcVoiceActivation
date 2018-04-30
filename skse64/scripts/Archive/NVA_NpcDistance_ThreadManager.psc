scriptname NVA_NpcDistance_ThreadManager extends Quest

Quest property ThreadQuest auto
Activator property FutureActivator auto
ObjectReference property FutureAnchor auto
Static property XMarker auto
Actor property thePlayer auto

NpcDistance_Thread_01 thread01
NpcDistance_Thread_02 thread02
NpcDistance_Thread_03 thread03
NpcDistance_Thread_04 thread04
NpcDistance_Thread_05 thread05
NpcDistance_Thread_06 thread06
NpcDistance_Thread_07 thread07
NpcDistance_Thread_08 thread08
NpcDistance_Thread_09 thread09
NpcDistance_Thread_10 thread10

float fTickLength = 0.1
int iMaxTicks = 100

event OnInit()
	RegisterForModEvent("NVA_OnCalculateDistance", "OnCalculateDistance")
	
	thread01 = ThreadQuest as NpcDistance_Thread_01
	thread02 = ThreadQuest as NpcDistance_Thread_02
	thread03 = ThreadQuest as NpcDistance_Thread_03
	thread04 = ThreadQuest as NpcDistance_Thread_04
	thread05 = ThreadQuest as NpcDistance_Thread_05
	thread06 = ThreadQuest as NpcDistance_Thread_06
	thread07 = ThreadQuest as NpcDistance_Thread_07
	thread08 = ThreadQuest as NpcDistance_Thread_08
	thread09 = ThreadQuest as NpcDistance_Thread_09
	thread10 = ThreadQuest as NpcDistance_Thread_10
endevent

ObjectReference function CalculateDistanceAsync(Actor akActor, sName)
	ObjectReference future
	while !future
		if !thread01.queued()
			future = thread01.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread02.queued()
			future = thread02.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread03.queued()
			future = thread03.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread04.queued()
			future = thread04.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread05.queued()
			future = thread05.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread06.queued()
			future = thread06.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread07.queued()
			future = thread07.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread08.queued()
			future = thread08.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread09.queued()
			future = thread09.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)
		elseif !thread10.queued()
			future = thread10.get_async(FutureActivator, FutureAnchor, akActor, XMarker, thePlayer, sName)			
		else
			wait_all()
		endif
	endwhile
	
	return future
endfunction

function wait_all()
	RaiseEvent_OnCalculateDistance()
	begin_waiting()
endfunction

function begin_waiting()
	bool waiting = true
	int i = 0
	while waiting
		if thread01.queued() || thread02.queued() || thread03.queued() || thread04.queued() || thread05.queued() || \
		   thread06.queued() || thread07.queued() || thread08.queued() || thread09.queued() || thread10.queued()
			i += 1
			Utility.wait(fTickLength)
			if i >= iMaxTicks
				debug.trace("ERROR: All threads have become unresponsive. Please debug this issue or notify the author.")
				i = 0
				return
			endif
		else
			waiting = false
		endif
	endwhile
endfunction

function TryToUnlockThread(ObjectReference akFuture)
	bool success = false
	if thread01.has_future(akFuture)
		success = thread01.force_unlock()
	elseif thread02.has_future(akFuture)
		success = thread02.force_unlock()
	elseif thread03.has_future(akFuture)
		success = thread03.force_unlock()
	elseif thread04.has_future(akFuture)
		success = thread04.force_unlock()
	elseif thread05.has_future(akFuture)
		success = thread05.force_unlock()
	elseif thread06.has_future(akFuture)
		success = thread06.force_unlock()
	elseif thread07.has_future(akFuture)
		success = thread07.force_unlock()
	elseif thread08.has_future(akFuture)
		success = thread08.force_unlock()
	elseif thread09.has_future(akFuture)
		success = thread09.force_unlock()
	elseif thread10.has_future(akFuture)
		success = thread10.force_unlock()		
	endif
	
	if !success
		debug.trace("ERROR: A thread has encountered an error and has become unresponsive.")
	else
		debug.trace("WARNING: An unresponsive thread was successfully unlocked.")
	endif
endfunction

function RaiseEvent_OnCalculateDistance()
	int handle = ModEvent.Create("NVA_OnCalculateDistance")
	if handle
		ModEvent.Send(handle)
	else
		;pass
	endif
endfunction