class SESOFCompass: SCR_InfoDisplay
{
	private World world = GetGame().GetWorld();
	
	// Compass: Image
	private FrameWidget SESOFCompassImageOnly = null;
	private ImageWidget SESOFCompassImageOnlyImage = null;
	
	// Compass: Bearing
	private FrameWidget SESOFCompassBearingOnly = null;
	private TextWidget SESOFCompassBearingOnlyBearing = null;
	private TextWidget SESOFCompassBearingOnlyDirection = null;
	
	// Compass Image & Bearing
	private FrameWidget SESOFCompassImageAndBearing = null;
	private ImageWidget SESOFCompassImageAndBearingImage = null;
	private TextWidget SESOFCompassImageAndBearingBearing = null;
	
	// Bools to switch between things
	private bool showCompass = true;
	private bool showCompassImage = true;
	private bool showCompassBearing = false;
	private bool compassImageDiscrete = false;
	
	override protected void OnInit(IEntity owner)
	{
		super.OnInit(owner);
		
		InitCompass();
		
		GetGame().GetInputManager().AddActionListener("SESOFCompassToggle", EActionTrigger.DOWN, ToggleCompass);
		GetGame().GetInputManager().AddActionListener("SESOFCompassToggleType", EActionTrigger.DOWN, ToggleCompassType);
		GetGame().GetInputManager().AddActionListener("SESOFCompassDiscreteBearings", EActionTrigger.DOWN, ToggleDiscreteBearings);
	}
	
	
	override protected void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);
		
		if (!world) return;
		
		// Fetch widgets if we haven't already.
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
		}
		UpdateCompass();
	}
	
	
	private void UpdateCompass()
	{
		if (!showCompass) {return;}
		
		// Get yaw
		float yaw = GetYaw();

		// Compass: Image
		if (showCompassImage && !showCompassBearing)
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
		}
		
		// Get direction and set up prefix of zeroes to draw bearing nicely
		string direction = YawToDirection(yaw);
		string yawTextPrefix = "";
		if (Math.Round(yaw) >= 0 & Math.Round(yaw) < 10) {yawTextPrefix = "00";}
		if (Math.Round(yaw) >= 10 & Math.Round(yaw) < 100) {yawTextPrefix = "0";}
		
		// Compass: Bearing
		if (showCompassBearing && !showCompassImage)
		{
			SESOFCompassBearingOnlyBearing.SetText(yawTextPrefix + Math.Round(yaw).ToString());
			SESOFCompassBearingOnlyDirection.SetText(direction);
		}
		
		// Compass Image & Bearing
		if (showCompassImage && showCompassBearing)
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
		}
	}
	

	
	// HELPERS BELOW HERE
	private void InitCompass()
	{
		Print("Compass initialised.");
		showCompass = true;
		showCompassImage = true;
		showCompassBearing = false;
	}
	
	
	private void ToggleCompassType()
	{
		if (showCompass)
		{
			// Compass: From Image to Bearing
			if (showCompassImage && !showCompassBearing)
			{
				SESOFCompassImageOnly.SetVisible(false);
				SESOFCompassBearingOnly.SetVisible(true);
				SESOFCompassImageAndBearing.SetVisible(false);
				
				showCompassImage = false;
				showCompassBearing = true;
				
				Print("Compass type: Bearing");
				return;
			}
			// Compass: From Bearing to Image and Bearing
			if (!showCompassImage && showCompassBearing)
			{	
				SESOFCompassImageOnly.SetVisible(false);
				SESOFCompassBearingOnly.SetVisible(false);
				SESOFCompassImageAndBearing.SetVisible(true);
				
				showCompassImage = true;
				showCompassBearing = true;
				
				Print("Compass type: Image and Bearing");
				return;
			}
			// Compass: From Image and Bearing to just Image
			if (showCompassImage && showCompassBearing)
			{
				SESOFCompassImageOnly.SetVisible(true);
				SESOFCompassBearingOnly.SetVisible(false);
				SESOFCompassImageAndBearing.SetVisible(false);
				
				showCompassImage = true;
				showCompassBearing = false;
				
				Print("Compass type: Image");
				return;
			}
		}
	}
	
	
	private void ToggleDiscreteBearings()
	{
		compassImageDiscrete = !compassImageDiscrete;
	}
	
	
	private void ToggleCompass()
	{
		// Hide the compass
		if (showCompass)
		{
			showCompass = false;
			
			SESOFCompassImageOnly.SetVisible(false);
			SESOFCompassBearingOnly.SetVisible(false);
			SESOFCompassImageAndBearing.SetVisible(false);
			Print("Compass hidden.");

			return;
		}
		
		// Show the compass
		if (!showCompass)
		{
			// Using the toggle-function requires the bools to be set to the previous slot
			showCompass = true;
			
			if (showCompassImage && !showCompassBearing)
			{
				SESOFCompassImageOnly.SetVisible(true);
				SESOFCompassBearingOnly.SetVisible(false);
				SESOFCompassImageAndBearing.SetVisible(false);
			}
			if (!showCompassImage && showCompassBearing)
			{
				SESOFCompassImageOnly.SetVisible(false);
				SESOFCompassBearingOnly.SetVisible(true);
				SESOFCompassImageAndBearing.SetVisible(false);
			}
			if (showCompassImage && showCompassBearing)
			{
				SESOFCompassImageOnly.SetVisible(false);
				SESOFCompassBearingOnly.SetVisible(false);
				SESOFCompassImageAndBearing.SetVisible(true);
			}
			Print("Compass shown.");
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
}