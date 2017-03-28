using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class MoveLightClass : MonoBehaviour {

    public float moveRange = 2f;

    private float m_x, m_y;

	// Use this for initialization
	void Start () {
	    m_x = transform.position.x;
	    m_y = transform.position.y;
	}
	
	// Update is called once per frame
	void Update () {
	    Quaternion q = Camera.main.transform.rotation;
	    transform.position.Set(m_x + q.x * moveRange, m_y, transform.position.z);
	}
}
