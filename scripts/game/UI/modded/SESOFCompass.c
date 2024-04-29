//---------------------------------------------------------------------------
class SESOFCompass: SCR_InfoDisplay
{
	//---------------------------------------------------------------------------
	private World world = GetGame().GetWorld();
	
	// Compass: Image
	private FrameWidget SESOFCompassImageOnly;
	private ImageWidget SESOFCompassImageOnlyImage;
	
	// Compass: Bearing
	private FrameWidget SESOFCompassBearingOnly;
	private TextWidget SESOFCompassBearingOnlyBearing;
	private TextWidget SESOFCompassBearingOnlyDirection;
	
	// Compass Image & Bearing
	private FrameWidget SESOFCompassImageAndBearing;
	private ImageWidget SESOFCompassImageAndBearingImage;
	private TextWidget SESOFCompassImageAndBearingBearing;
	
	private bool showCompass = false;
	private bool compassImageDiscrete = false;
	private int currentCompassType = 0;
	
	override protected void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);
		
		if (!world) return;
		
		// Fetch widgets and initialise compass on the first update.
		if (!SESOFCompassImageOnly) 
		{
			// Compass: Image
			SESOFCompassImageOnly = FrameWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassImageOnly"));
			SESOFCompassImageOnlyImage = ImageWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassImageOnlyImage"));
	
			// Compass: Bearing
			SESOFCompassBearingOnly = FrameWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassBearingOnly"));
			SESOFCompassBearingOnlyBearing = TextWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassBearingOnlyBearing"));
			SESOFCompassBearingOnlyDirection = TextWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassBearingOnlyDirection"));
	
			// Compass Image & Bearing
			SESOFCompassImageAndBearing = FrameWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassImageAndBearing"));
			SESOFCompassImageAndBearingImage = ImageWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassImageAndBearingImage"));
			SESOFCompassImageAndBearingBearing = TextWidget.Cast(m_wRoot.FindAnyWidget("SESOFCompassImageAndBearingBearing"));
			
			LoadCompassConfig();	
			GetGame().GetInputManager().AddActionListener("SESOFCompassToggle", EActionTrigger.DOWN, ToggleCompass);
		}
		
		// Update the compass
		UpdateCompassValues();
	}
	
	
	private void UpdateCompassValues()
	{
		if (!showCompass) {return;}
		if (currentCompassType == 0) {return;}
		
		// Get yaw
		float yaw = GetYaw();

		// Compass: Image
		if (currentCompassType == 3)
		{
			if (!compassImageDiscrete)
			{
				float compassImagePosX
				if (yaw < 245) {compassImagePosX = -720 - (yaw * 2)};
				if (yaw > 244) {compassImagePosX = -(yaw * 2)};
				FrameSlot.SetPosX(SESOFCompassImageOnlyImage, compassImagePosX)
			}		
			if (compassImageDiscrete)
			{
				float compassImagePosX
				if (Math.Round(yaw) < 245) {compassImagePosX = -720 - (Math.Round(yaw) * 2)};
				if (Math.Round(yaw) > 244) {compassImagePosX = -(Math.Round(yaw) * 2)};
				FrameSlot.SetPosX(SESOFCompassImageOnlyImage, compassImagePosX)
			}
			return;
		}
		
		// Get direction and set up prefix of zeroes to draw bearing nicely
		string direction = YawToDirection(yaw);
		string yawTextPrefix = "";
		if (Math.Round(yaw) >= 0 & Math.Round(yaw) < 10) {yawTextPrefix = "00";}
		if (Math.Round(yaw) >= 10 & Math.Round(yaw) < 100) {yawTextPrefix = "0";}
		
		// Compass: Bearing
		if (currentCompassType == 1)
		{
			SESOFCompassBearingOnlyBearing.SetText(yawTextPrefix + Math.Round(yaw).ToString());
			SESOFCompassBearingOnlyDirection.SetText(direction);
			return;
		}

		// Compass Image & Bearing
		if (currentCompassType == 2)
		{
			SESOFCompassImageAndBearingBearing.SetText(yawTextPrefix + Math.Round(yaw).ToString());
			if (!compassImageDiscrete)
			{
				float compassImagePosX
				if (yaw < 245) {compassImagePosX = -720 - (yaw * 2)};
				if (yaw > 244) {compassImagePosX = -(yaw * 2)};
				FrameSlot.SetPosX(SESOFCompassImageAndBearingImage, compassImagePosX)
			}			
			if (compassImageDiscrete)
			{
				float compassImagePosX
				if (Math.Round(yaw) < 245) {compassImagePosX = -720 - (Math.Round(yaw) * 2)};
				if (Math.Round(yaw) > 244) {compassImagePosX = -(Math.Round(yaw) * 2)};
				FrameSlot.SetPosX(SESOFCompassImageAndBearingImage, compassImagePosX)
			}
			
			return;
		}
	}
	
	
	protected string YawToDirection(int bearing)
	{
		string directions[8] = {"N", "NE", "E", "SE", "S", "SW", "W", "NW"};
		int result = Math.Round(bearing / 45);
		if (result == 8) 
		{
			return directions[0];
		}
		else
			return directions[result];
	}
	
	
	protected float GetYaw()
	{
		vector transform[4];
		world.GetCurrentCamera(transform);
		float yaw = Math3D.MatrixToAngles(transform)[0];
		if (yaw < 0) {yaw = 360 - Math.AbsFloat(yaw);}
		return yaw;
	}
		

	// Shows / hides compass
	private void ToggleCompass()
	{
		if (!showCompass)
		{
			showCompass = true;
			
			GetGame().GetInputManager().AddActionListener("SESOFCompassToggleType", EActionTrigger.DOWN, ToggleCompassType);
			GetGame().GetInputManager().AddActionListener("SESOFCompassDiscreteBearings", EActionTrigger.DOWN, ToggleDiscreteBearings);
			
			ShowCompassType(currentCompassType);
			SaveCompassConfig();
			return;
		}
		
		if (showCompass) 
		{
			showCompass = false;
			
			GetGame().GetInputManager().RemoveActionListener("SESOFCompassToggleType", EActionTrigger.DOWN, ToggleCompassType);
			GetGame().GetInputManager().RemoveActionListener("SESOFCompassDiscreteBearings", EActionTrigger.DOWN, ToggleDiscreteBearings);
			
			ShowCompassType(0);
			SaveCompassConfig();
		}
	}
	

	// Toggles between the three available types
	private void ToggleCompassType()
	{
		currentCompassType += 1;
		if (currentCompassType == 4) {currentCompassType = 1;}
		ShowCompassType(currentCompassType);
		SaveCompassConfig();
	}
	
	
	// More clickety-feel if discreet.
	private void ToggleDiscreteBearings()
	{
		compassImageDiscrete = !compassImageDiscrete;
		SaveCompassConfig();
	}
	
	
	// Shows/hides different compass types including hiding them all.
	private void ShowCompassType(int compassType)
	{	
		// Hide
		if (compassType == 0)
		{
			SESOFCompassImageOnly.SetVisible(false);
			SESOFCompassBearingOnly.SetVisible(false);
			SESOFCompassImageAndBearing.SetVisible(false);
		}
		
		// Bearing only
		if (compassType == 1)
		{
			SESOFCompassImageOnly.SetVisible(false);
			SESOFCompassBearingOnly.SetVisible(true);
			SESOFCompassImageAndBearing.SetVisible(false);		
		}
		
		// Image with bearing below
		if (compassType == 2)
		{
			SESOFCompassImageOnly.SetVisible(false);
			SESOFCompassBearingOnly.SetVisible(false);
			SESOFCompassImageAndBearing.SetVisible(true);			
		}
	
		// Image only
		if (compassType == 3)
		{
			SESOFCompassImageOnly.SetVisible(true);
			SESOFCompassBearingOnly.SetVisible(false);
			SESOFCompassImageAndBearing.SetVisible(false);
		}	
	}

	
	private void SaveCompassConfig()
	{
		//Print("Saving compass config.");
		SESOFCompassConfig configFile = new SESOFCompassConfig();
		
		configFile.compassType = currentCompassType;
		configFile.showDiscreteBearings = compassImageDiscrete;
		configFile.showCompass = showCompass;
		configFile.touched = true;
		
		configFile.SaveToFile("SESOFCompassConfig.json");
		
		Print("Saved currentCompassType: " + currentCompassType);
		Print("Saved showDiscreteBearings: " + compassImageDiscrete);
		Print("Saved showCompass: " + showCompass);
	}
	
	
	private void LoadCompassConfig()
	{
		//Print("Loading compass config.");
		SESOFCompassConfig configFile = new SESOFCompassConfig();
		
		// LoadFromFile simply fails silently, so added a touched-variable to enable checks.
		configFile.LoadFromFile("SESOFCompassConfig.json");
		if (configFile.touched == 0) 
		{
			Print("SESOF Compass Failed to load config. Creating new default.");
			showCompass = true;
			currentCompassType = 3;
			compassImageDiscrete = false;
			SaveCompassConfig();
			//configFile.LoadFromFile("SESOFCompassConfig.json");
			LoadCompassConfig();
		}

		currentCompassType = configFile.compassType;	
		if (configFile.showCompass == 1) {showCompass = true;}
		if (configFile.showDiscreteBearings == 1) {compassImageDiscrete = true;}
		
			
		Print("Loaded currentCompassType: " + currentCompassType);
		Print("Loaded showDiscreteBearings: " + compassImageDiscrete);
		Print("Loaded showCompass: " + showCompass);
		
		if (showCompass)
		{
			Print("Loaded config with compass on.");
			GetGame().GetInputManager().AddActionListener("SESOFCompassToggleType", EActionTrigger.DOWN, ToggleCompassType);
			GetGame().GetInputManager().AddActionListener("SESOFCompassDiscreteBearings", EActionTrigger.DOWN, ToggleDiscreteBearings);
			
			Print("Showing compass type " + currentCompassType);
			ShowCompassType(currentCompassType);
		}
		
		if (!showCompass) 
		{
			Print("Loaded config with compass off.");
			GetGame().GetInputManager().RemoveActionListener("SESOFCompassToggleType", EActionTrigger.DOWN, ToggleCompassType);
			GetGame().GetInputManager().RemoveActionListener("SESOFCompassDiscreteBearings", EActionTrigger.DOWN, ToggleDiscreteBearings);
			
			ShowCompassType(0);
		}
	}
}


class SESOFCompassConfig : JsonApiStruct
{
	int compassType;
	bool showDiscreteBearings;
	bool showCompass;
	bool touched = false;


	void SESOFCompassConfig()
	{
		RegV("compassType");
		RegV("showDiscreteBearings");
		RegV("showCompass");
		RegV("touched");
	}
}