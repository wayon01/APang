using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GoalTileObject : TileObject {

    private APlayer player;
    private GameSystemMgr gameSystemMgr;
    private int stageNextId;


    void Awake() {
        stageNextId = -1;
    }
	// Use this for initialization
    private void Start () {
        player = GameObject.Find("player").GetComponent<APlayer>();
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
	}
	
	// Update is called once per frame
	void Update () {

	}

    public override void SetLocalX(float x) {
        base.SetLocalX(x);
        //transform.position = new Vector3(x, transform.position.y, transform.position.z);
        if (player.positionId.y == id.y && player.positionId.z == id.z) {
            if (stageNextId == -2) return;
            if (gameSystemMgr.m_switchCount != gameSystemMgr.m_switchMaximum) return;
            gameSystemMgr.isCleared = true;
            gameSystemMgr.m_nextStageId = stageNextId;
        }
    }

    public override void SetLocalZ(float z) {
        base.SetLocalZ(z);
        //transform.position = new Vector3(transform.position.x, transform.position.y, z);
        if (player.positionId.y == id.y && player.positionId.x == id.x) {
            if (stageNextId == -2) return;
            if (gameSystemMgr.m_switchCount != gameSystemMgr.m_switchMaximum) return;
            gameSystemMgr.isCleared = true;
            gameSystemMgr.m_nextStageId = stageNextId;
        }
    }

    public void SetNextStageId(int id) {
        stageNextId = id;
    }

    public override void Init() {
        base.Init();
        
    }
}
