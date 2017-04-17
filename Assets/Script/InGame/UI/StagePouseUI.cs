using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StagePouseUI : MonoBehaviour {

    public bool isPause = false;

    private TweenScale tweenScale;
    private GameSystemMgr gameSystemMgr;

	// Use this for initialization
	void Start () {
	    isPause = false;
	    tweenScale = GetComponent<TweenScale>();
	    gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        transform.localScale = new Vector3(0.5f, 0, 0.5f);
	}
	
	// Update is called once per frame
	void Update () {

	}

    public void OnClickedEvent() {
        if (gameSystemMgr.isFailed || gameSystemMgr.isCleared) return;

        gameSystemMgr.isPause = isPause;

        if (isPause && !tweenScale.enabled) {
            tweenScale.enabled = true;

            tweenScale.@from = new Vector3(0.5f, 0, 0.5f);
            tweenScale.to = Vector3.one;

            tweenScale.Reset();
        }
        else if (!isPause && !tweenScale.enabled) {
            tweenScale.enabled = true;

            tweenScale.@from = Vector3.one;
            tweenScale.to = new Vector3(0.5f, 0, 0.5f);

            tweenScale.Reset();
        }
    }

    public void SetPause(bool pause) {
        if (gameSystemMgr.isFailed || gameSystemMgr.isCleared) return;
        isPause = gameSystemMgr.isPause = pause;
    }
}
