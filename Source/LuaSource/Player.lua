
FirePower = 1000
Health = 100

CanAttack = true

AttackCooldown = 3
CurrentCooldown = 0

MovmentMul = 0.5

SprintMul = 1

SprintDecay = 10
SprintCharge = 100

StaminaCurrent = 100
StaminaMax = 100

StaminaCoolDown = 0

IsSprinting = false

IsMoving = false
IsADS = false

IsHit = true

Slowdown = 0

function Tick(DeltaTime)
	if IsSprinting then
        StaminaCurrent = math.max(StaminaCurrent - (SprintDecay * DeltaTime), 0)

        if StaminaCurrent == 0 then
            StopSprint(true)
        end
    elseif StaminaCoolDown > 0 then 
    	StaminaCoolDown = StaminaCoolDown - DeltaTime

    elseif not IsSprinting then
       	StaminaCurrent = math.min(StaminaCurrent + (SprintCharge * DeltaTime), StaminaMax)
	end

    if not IsMoving and IsSprinting then
    	StopSprint()
    end

    if Slowdown > 0 then
    	Slowdown = Slowdown - DeltaTime
    end
end

function GetSpeedMultiplier()
	local mul = 1

	if IsADS then
        return SprintMul * 0.5
	end

	if Slowdown > 0 then
		return SprintMul * 0.5
	end

	return SprintMul
end

function StartSprint()
	if StaminaCurrent > 10 then
	    SprintMul = 5
	    IsSprinting = true
    end
end

function CanSprint()
	return StaminaCoolDown <= 0 and StaminaCurrent > 10 and IsMoving
end

function SlowPlayer(TimeToSlow)
	ue4.print("SlowPlayer")
	Slowdown = TimeToSlow
end

function StopSprint(cooldown)
	SprintMul = 1
	IsSprinting = false
	if cooldown then
		StaminaCoolDown = 3
	end
end

function GetSpreadMultiplier()
    if IsADS then 
    	return 0.3
    end

    return 1
end

function GetADSSpeed()
	return 20
end
