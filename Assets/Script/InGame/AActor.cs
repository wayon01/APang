using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class AActor : AGameObject {
    public Vector3 positionId;

	// Use this for initialization
	void Start () {
		positionId = new Vector3(-1, -1, -1);
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    public virtual void Die() {
        
    }

}
