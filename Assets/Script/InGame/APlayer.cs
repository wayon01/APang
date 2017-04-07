using System.Collections;
using System.Collections.Generic;
using UnityEditor.Animations;
using UnityEngine;

public class APlayer : AActor {

    public bool isBlocked;
    public Animator animator;

	// Use this for initialization
	void Start () {
	    isBlocked = false;
	}
	
	// Update is called once per frame
	void Update () {
	}

    void LateUpdate() {
        Quaternion q = Camera.main.transform.rotation;
        if(!Input.GetMouseButtonDown(0) && !Input.GetMouseButton(0))
            transform.rotation = Quaternion.Lerp(transform.rotation, Camera.main.transform.rotation, 0.1f);
    }

    public override void Die() {
        
    }

    
}
