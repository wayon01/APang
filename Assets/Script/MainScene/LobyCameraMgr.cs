using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobyCameraMgr : MonoBehaviour {

    public GameObject MainCamera;
    public float Sensitivity = 200f;
    public bool isRelax;
    public int MaximumStageId = 2;
    public bool isSameStage;

    public float FinalPositionX = 0f;

    private float mouse_delta_x;
    private float tempPosition_x;
    private int prev_stageId;
    public int stageId;


    private DataMgr dataMgr;


    // Use this for initialization
    void Start () {
        dataMgr = GameObject.Find("DataManager").GetComponent<DataMgr>();
        
        mouse_delta_x = 0;
	    isRelax = true;
	    stageId = dataMgr.dataClass.CURRENT_STAGE_ID;
        FinalPositionX = GetFinalPositionX(stageId);
        transform.localPosition = new Vector3(FinalPositionX, transform.localPosition.y,
            transform.localPosition.z);
        isSameStage = false;

	}
	
	// Update is called once per frame
	void Update () {
	    if (Input.GetMouseButtonDown(0)) {
	        tempPosition_x = 0;
            mouse_delta_x = -Input.mousePosition.x/Sensitivity;
	        prev_stageId = stageId;

	    }

	    if (Input.GetMouseButton(0)) {
	        tempPosition_x += -Input.mousePosition.x/Sensitivity - mouse_delta_x;
	        mouse_delta_x = -Input.mousePosition.x/Sensitivity;

            if (FinalPositionX + tempPosition_x < 0) {
                tempPosition_x = 0;
            }
            else if (FinalPositionX + tempPosition_x > 46.39f) {
                tempPosition_x = 46.39f - FinalPositionX;
            }

	        float tempPosition_x_abs = Math.Abs(tempPosition_x);

            if (tempPosition_x_abs > 0.01f) {

	            isRelax = false;
	            transform.position = new Vector3(FinalPositionX + tempPosition_x,
	                transform.position.y, transform.position.z);

                if (tempPosition_x_abs > 6f) {
                    if (stageId != prev_stageId) return;
                    stageId = tempPosition_x < 0 ? stageId - 1 : stageId + 1;

                    if (stageId < 0) stageId = 0;
                    else if (stageId > MaximumStageId) stageId = MaximumStageId;

                }
                else if(stageId != prev_stageId) {
                    stageId = prev_stageId;
                }

	        }

            isSameStage = stageId == prev_stageId;


        }
        else if (Input.GetMouseButtonUp(0)) {
            tempPosition_x = 0;
	        FinalPositionX = GetFinalPositionX(stageId);

	    }
	    else {
	        float ResultX = FinalPositionX;
            transform.position = new Vector3(Mathf.Lerp(transform.position.x, ResultX, Time.deltaTime * 15),
                    transform.position.y, transform.position.z);

	        if (!isRelax) {
	            isRelax = ResultX - transform.position.x < 0.0001f;
	        }
        }
	}

    public float GetTempPosition() {
        return tempPosition_x;
    }

    private float GetFinalPositionX(int _stageId) {
        switch (_stageId) {
            case 0:
                return 8f;
            case 1:
                return 27.15f;
            case 2:
                return 46.39f;
        }

        return 0;
    }
}
