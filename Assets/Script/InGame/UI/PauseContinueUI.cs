using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PauseContinueUI : MonoBehaviour {

    public GameObject pouse_panel;
    private StagePouseUI _stagePouseUi;

	// Use this for initialization
	void Start () {
	    _stagePouseUi = pouse_panel.GetComponent<StagePouseUI>();
	}
	
	// Update is called once per frame
	void Update () {
		
	}

    void OnClick() {
        _stagePouseUi.SetPause(false);
        _stagePouseUi.OnClickedEvent();
    }
}
