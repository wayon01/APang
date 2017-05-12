using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ClearStarsUI : MonoBehaviour {

    private GameSystemMgr gameSystemMgr;

    private ClearStarUI[] stars;

    private float accrueTime;

    // Use this for initialization
    void Start() {
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        stars = new ClearStarUI[3];
        accrueTime = 0;

        for (int i = 0; i < transform.childCount; i++) {
            GameObject tmp = transform.GetChild(i).gameObject;

            if (tmp.transform.name == "clear_star_1") {
                stars[0] = tmp.GetComponent<ClearStarUI>();
                continue;
            }

            if (tmp.transform.name == "clear_star_2") {
                stars[1] = tmp.GetComponent<ClearStarUI>();
                continue;

            }

            if (tmp.transform.name == "clear_star_3") {
                stars[2] = tmp.GetComponent<ClearStarUI>();
                continue;

            }
        }
    }

    // Update is called once per frame
	void Update () {

	    if (!stars[0].isEnable && accrueTime >= 1) {
                stars[0].SetEnable(true);
	    }

        if (!stars[1].isEnable && accrueTime >= 1.3f) {
            if (gameSystemMgr.m_playerScore > 1) {
                stars[1].SetEnable(true);
            }
        }

        if (!stars[2].isEnable && accrueTime >= 2f) {
            if (gameSystemMgr.m_playerScore > 2) {
                stars[2].SetEnable(true);
            }
        }

        accrueTime += Time.deltaTime;

	}
}
