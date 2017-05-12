using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StageGaugeBar : MonoBehaviour {

    public bool b_midGauge;
    public bool b_topGauge;
    private GameSystemMgr gameSystemMgr;
    private TileMgr tileMgr;
    private TweenScale tweenScale;

    private bool isInited;


    // Use this for initialization
    void Start () {
        b_midGauge = b_topGauge = true;
        isInited = false;
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        tileMgr = GameObject.Find("TileManager").GetComponent<TileMgr>();
        tweenScale = GetComponent<TweenScale>();

    }
	
	// Update is called once per frame
	void Update () {

	    if (!isInited && tileMgr.isLoad) {
	        isInited = true;

            GameObject.Find("grade_top").GetComponent<GaugeStar>().SetValue(gameSystemMgr.m_playerMovementMaximum[0]);
            GameObject.Find("grade_mid").GetComponent<GaugeStar>().SetValue(gameSystemMgr.m_playerMovementMaximum[1]);


        }

        if (b_topGauge && gameSystemMgr.m_playerMovingCount > gameSystemMgr.m_playerMovementMaximum[0]) {
	        b_topGauge = false;
	        GameObject.Find("grade_top").GetComponent<GaugeStar>().SetClosed();
            SetTweenScale(new Vector3(174, 18, 1), new Vector3(87, 18, 1));
            gameSystemMgr.m_playerScore = 2;
        }

        if (b_midGauge && gameSystemMgr.m_playerMovingCount > gameSystemMgr.m_playerMovementMaximum[1]) {
            b_midGauge = false;
            GameObject.Find("grade_mid").GetComponent<GaugeStar>().SetClosed();
            SetTweenScale(transform.localScale, new Vector3(0, 18, 1));
            gameSystemMgr.m_playerScore = 1;
        }


    }

    private void SetTweenScale(Vector3 from, Vector3 to) {
        tweenScale.enabled = true;

        tweenScale.@from = from;
        tweenScale.to = to;

        tweenScale.Reset();
    }
}
