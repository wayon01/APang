using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class APlayer : AActor {

    public bool isBlocked;

	// Use this for initialization
	void Start () {
	    isBlocked = false;
	}
	
	// Update is called once per frame
	void Update () {
	}

    void LateUpdate() {
        Quaternion q = Camera.main.transform.rotation;
        transform.rotation = q;
    }

    public override void Die() {
        
    }
}
