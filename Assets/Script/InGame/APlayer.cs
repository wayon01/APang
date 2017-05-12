using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class APlayer : AActor {

    public bool isBlocked;
    public Animator animator;

    private bool isInPortal;
    private Vector3 targetScale;
    private float PortalTime;

	// Use this for initialization
	void Start () {
	    isBlocked = false;
	    isInPortal = false;
        targetScale = Vector3.one;
	}
	
	// Update is called once per frame
	void Update () {

	    if (transform.localScale != targetScale) {
	        PortalTime += Time.deltaTime;

	        transform.localScale = Vector3.Lerp(transform.localScale, targetScale, PortalTime);
	    }

	}

    void LateUpdate() {
        Quaternion q = Camera.main.transform.rotation;
        if(!Input.GetMouseButtonDown(0) && !Input.GetMouseButton(0))
            transform.rotation = Quaternion.Lerp(transform.rotation, Camera.main.transform.rotation, 0.1f);
    }

    public override void Die() {
        
    }

    public void OnTakePortal() {

        isInPortal = !isInPortal;

        animator.SetBool("isDisappear", isInPortal);
        PortalTime = 0;

        if (isInPortal) {
            targetScale = Vector3.zero;
            PortalTime = -0.5f;
        }
        else {
            targetScale = Vector3.one;
        }

    }

}
