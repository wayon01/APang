using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StageResultUI : MonoBehaviour {

    private GameSystemMgr gameSystemMgr;
    private float accrueTime = 0;
    private GameObject stage_result;
    

    // Use this for initialization
    void Start () {
	    gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        stage_result = GameObject.Find("stage_result");
        accrueTime = 0;
    }
	
	// Update is called once per frame
	void Update () {
	    if (gameSystemMgr.isCleared || gameSystemMgr.isFailed) {

	        if (accrueTime == 0 && gameSystemMgr.isFailed) {
	            stage_result.GetComponent<UISprite>().spriteName = "result_txt_over";
	        }
	        accrueTime += Time.deltaTime;

	        if (gameSystemMgr.isFailed && accrueTime > 1.5f ||
                gameSystemMgr.isCleared && accrueTime > 1f) {
	            Vector3 position = transform.localPosition;
	            transform.localPosition = new Vector3(
                    0,
                    Mathf.Lerp(position.y, 0, 0.1f),
                    0
                    );
	        }
	    }
	}
}
