using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StagePauseBt : MonoBehaviour {

    public GameObject pauseUI;

    private StagePauseUI _stagePauseUi;

	// Use this for initialization
	void Start () {
	    _stagePauseUi = pauseUI.GetComponent<StagePauseUI>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnClick() {
        _stagePauseUi.isPause = !_stagePauseUi.isPause;
        _stagePauseUi.OnClickedEvent();
    }
}
