﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class BillboardImage : MonoBehaviour {

	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
    }

    void LateUpdate() {
        transform.rotation = Camera.main.transform.rotation;
    }
}
