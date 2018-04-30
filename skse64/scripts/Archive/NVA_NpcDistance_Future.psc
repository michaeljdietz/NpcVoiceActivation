scriptname NVA_NpcDistance_Future extends ObjectReference

Quest property NpcDistance_Quest auto

float fTickLength = 0.1
int iMaxTicks = 100

float r
float property distance hidden
	function set(float fResult)
		done = true
		r = fResult
	endfunction
endproperty

bool done = false

bool function done()
	return done
endfunction

ObjectReference function get_result()
	int i = 0
	while !done && i < iMaxTicks
		i += 1
		Utility.wait(fTickLength)
	endwhile
	RegisterForSingleUpdate(fTickLength)
	
	if i >= iMaxTicks
		(NpcDistance_Quest as NVA_NpcDistance_ThreadManager).TryToUnlockThread(self as ObjectReference)
	endif
	return r
endfunction

event OnUpdate()
	self.Disable()
	self.Delete()
endevent