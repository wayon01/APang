using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseContinueUI : MonoBehaviour {

    public GameObject pouse_panel;
    private StagePauseUI _stagePauseUi;

	// Use this for initialization
	void Start () {
	    _stagePauseUi = pouse_panel.GetComponent<StagePauseUI>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnClick() {
        _stagePauseUi.SetPause(false);
        _stagePauseUi.OnClickedEvent();
    }
}
