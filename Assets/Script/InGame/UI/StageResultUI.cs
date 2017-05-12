using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class StageResultUI : MonoBehaviour {

    private GameSystemMgr gameSystemMgr;
    private float accrueTime = 0;
    private GameObject stage_result;

    private GameObject clear_background;
    private GameObject layer2;
    private GameObject particle;



    // Use this for initialization
    void Start () {
        transform.localPosition= new Vector3(0, 200, 0);
	    gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        clear_background = GameObject.Find("clear_background");
        particle = GameObject.Find("clear_background_particle");
        particle.SetActive(false);
        //stage_result = GameObject.Find("stage_result");
        accrueTime = 0;
    }
	
	// Update is called once per frame
	void Update () {
	    if (gameSystemMgr.isCleared || gameSystemMgr.isFailed) {

	        if (accrueTime == 0 && gameSystemMgr.isFailed) {
	            //stage_result.GetComponent<UISprite>().spriteName = "result_txt_over";
            }
            else if (accrueTime == 0 && gameSystemMgr.isCleared) {
                transform.localPosition = Vector3.zero;
	            clear_background.GetComponent<TweenScale>().enabled = true;
	        }

	        if (gameSystemMgr.isCleared) {
	            if (layer2 == null && accrueTime >= 0.5f) {
                    layer2 = GameObject.Find("clear_panel_layer_2");
	                layer2.GetComponent<TweenScale>().enabled = true;
                    layer2.GetComponent<ClearStarsUI>().enabled = true;
                    particle.SetActive(true);
                }
	        }

            accrueTime += Time.deltaTime;
        }
	}
}
