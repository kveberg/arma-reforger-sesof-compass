class SESOF_Compass: SCR_InfoDisplay
{
	private TextWidget compassText = null;
	private ImageWidget compassImage = null;
	
	
	override protected void UpdateValues(IEntity owner, float timeSlice)
	{
		super.UpdateValues(owner, timeSlice);
		
		SCR_ChimeraCharacter character = SCR_ChimeraCharacter.Cast(SCR_PlayerController.GetLocalControlledEntity());
		
		if (!compassText || !compassImage) {
			compassText = TextWidget.Cast(m_wRoot.FindAnyWidget("SESOF_compassText"));
			compassImage = ImageWidget.Cast(m_wRoot.FindAnyWidget("SESOF_compassImage"));
		};
	
		UpdateCompass(compassText, compassImage);
	}
	
	protected void UpdateCompass(TextWidget compassTextRef, ImageWidget compassImageRef)
	{
		World world = GetGame().GetWorld();
		if (!world) return;
		
		vector transform[4];
		world.GetCurrentCamera(transform);
		
		int yaw = Math3D.MatrixToAngles(transform)[0];
		if (yaw < 0) {yaw = 360 - Math.AbsFloat(yaw);}
		
		string compassTextAdd = "";
		if (yaw >= 0 & yaw < 10) {compassTextAdd = "00";}
		if (yaw >= 10 & yaw < 100) {compassTextAdd = "0";}
		string compassTextSTR = compassTextAdd + (yaw.ToString());
		
		compassTextRef.SetText(compassTextSTR);
		
		/*
		Look at the layout to see what is happening ... -720 is the middle of the image, and one degree is 2px as the image is 1440px.
		*/
		float compassImagePosX
		if (yaw < 245) {compassImagePosX = -720 - (yaw * 2)};
		if (yaw > 244) {compassImagePosX = -(yaw * 2)};
		FrameSlot.SetPosX(compassImageRef, compassImagePosX)
	}
	
	
	
};