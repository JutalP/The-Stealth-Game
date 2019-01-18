Slowdown = 0

function Tick(DeltaTime)
    if Slowdown > 0 then
    	Slowdown = Slowdown - DeltaTime
    end
end

function GetSpeedMultiplier()
	local mul = 1

	if Slowdown > 0 then
		mul = mul * 0.1
	end

	return mul
end

function SlowPlayer(TimeToSlow)
	ue4.print("SlowBot")
	Slowdown = TimeToSlow
end