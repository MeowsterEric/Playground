package observer_observable;

import java.util.Observable;

public class PointsCard extends Observable{
	
	private int numPoints;
	
	public PointsCard(){
		numPoints = 0;
	}
	
	public void addPoints(int n){
		numPoints += n;
	}
	
	public void redeemPoints(int n){
		numPoints -= n;
	}

}
