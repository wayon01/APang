using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BackgroundImageClass : MonoBehaviour {

    public GameObject mainCamera;
    private Vector3 Position;

	// Use this for initialization
	void Start () {
	    Position = transform.position;
	}
	
	// Update is called once per frame
	void Update () {
	    transform.position.Set(mainCamera.transform.position.x, Position.y, Position.z);
	}
}
