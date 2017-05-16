using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class LobyStageMgr : MonoBehaviour {

    public GameObject EpisodeTitleId = null;
    public GameObject EpisodeTitleName = null;

    public GameObject StageInfo = null;

    public int CurrentStageId;
    public string[] StageTitle;

    private LobyCameraMgr cameraMgr;
    private DataMgr dataMgr;
    private Vector3 UIPosition;

    private Vector3 EpisodeTitleIdPos;
    private Vector3 EpisodeTitleNamePos;
    private Vector3 StageInfoPos;

    private bool isMouseUp;

    // Use this for initialization
    void Start () {
	    cameraMgr = GameObject.Find("CameraManager").GetComponent<LobyCameraMgr>();
        dataMgr = GameObject.Find("DataManager").GetComponent<DataMgr>();

        EpisodeTitleIdPos = EpisodeTitleId.transform.localPosition;
        EpisodeTitleNamePos = EpisodeTitleName.transform.localPosition;
        StageInfoPos = StageInfo.transform.localPosition;
        StageTitle = new string[cameraMgr.MaximumStageId + 1];

        {
            StageTitle[0] = "~안녕하세요 여러분!!~";
            StageTitle[1] = "~정원이에요!!!!~";
            StageTitle[2] = "~헤헤 유니티 백작님 어디게세여??~";

        }

        isMouseUp = false;
        CurrentStageId = dataMgr.dataClass.CURRENT_STAGE_ID + 1;

        EpisodeTitleId.GetComponent<UILabel>().text = "Episode " + CurrentStageId;
        EpisodeTitleName.GetComponent<UILabel>().text = StageTitle[CurrentStageId - 1];
    }

    // Update is called once per frame
    void Update () {
	    if (!cameraMgr.isRelax) {

            if (Input.GetMouseButton(0)) {
                float position = -cameraMgr.GetTempPosition();
                UIPosition.x = position * (Mathf.Abs(position) > 0.5f ? 200 : 1);
            }
            else if (Input.GetMouseButtonUp(0)) {
                isMouseUp = true;
                CurrentStageId = cameraMgr.stageId + 1;
                dataMgr.dataClass.CURRENT_STAGE_ID = cameraMgr.stageId;
                dataMgr.SaveData();
                if (!cameraMgr.isSameStage) {
                    UIPosition.x = UIPosition.x < 0 ? 500 : -500;
                    EpisodeTitleId.GetComponent<UILabel>().text = "Episode " + CurrentStageId;
                    EpisodeTitleName.GetComponent<UILabel>().text = StageTitle[CurrentStageId - 1];
                }

            }


            SetPosition();

	    }

        if (isMouseUp) {
            UIPosition.x = Mathf.Lerp(UIPosition.x, 0, Time.deltaTime * 15);
            SetPosition();
            if (Mathf.Abs(UIPosition.x) < 0.001f) {
                isMouseUp = false;
                UIPosition.x = 0;
            }
        }
	}

    private void SetPosition() {
        EpisodeTitleId.transform.localPosition = EpisodeTitleIdPos + UIPosition;
        EpisodeTitleName.transform.localPosition = EpisodeTitleNamePos + UIPosition;
        StageInfo.transform.localPosition = StageInfoPos + UIPosition;
    }
}
