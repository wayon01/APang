using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StagePauseBt : MonoBehaviour {

    public GameObject pauseUI;

    private StagePouseUI stagePouseUi;

	// Use this for initialization
	void Start () {
	    stagePouseUi = pauseUI.GetComponent<StagePouseUI>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnClick() {
        stagePouseUi.isPause = !stagePouseUi.isPause;
        stagePouseUi.OnClickedEvent();;
    }
}
