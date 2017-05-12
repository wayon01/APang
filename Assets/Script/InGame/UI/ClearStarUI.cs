using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClearStarUI : MonoBehaviour {

    private GameObject star;
    private GameObject particle;

    private float accrueTime;

    public bool isEnable;

	// Use this for initialization
	void Start () {

	    isEnable = false;
	    accrueTime = 0;

        for (int i = 0; i < transform.childCount; i++) {
            GameObject tmp = transform.GetChild(i).gameObject;

            if (tmp.transform.name == "star") {
                star = tmp;
                continue;
            }

            if (tmp.transform.name == "particle") {
                particle = tmp;
                continue;
            }
        }
    }
	
	// Update is called once per frame
	void Update () {
	    if (isEnable && accrueTime < 1) {
            accrueTime += Time.deltaTime;

	        if (accrueTime >= 0.5f) {
	            particle.SetActive(true);
	        }

        }
	}


    public void SetEnable(bool enable) {
        isEnable = enable;

        if (enable) {
            star.SetActive(true);
        }
    }

}
