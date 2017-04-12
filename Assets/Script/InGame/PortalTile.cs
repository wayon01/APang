using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PortalTile : TileObject {
    
    private APlayer player;
    private GameSystemMgr gameSystemMgr;
    private StageMgr stageMgr;

    private Vector3 targetTileId;
    private int targetStage;

    private bool isPortalActived;

    void Awake() {
        targetTileId = -Vector3.one;
        targetStage = -1;
        isPortalActived = false;
    }

    // Use this for initialization
    void Start () {
        player = GameObject.Find("player").GetComponent<APlayer>();
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        stageMgr = GameObject.Find("StageManager").GetComponent<StageMgr>();

        
    }
	
	// Update is called once per frame
	void Update () {
	    if (!isPortalActived && gameSystemMgr.isPlayerMovingUp) {
	        isPortalActived = true;
	    }
	}

    public override void SetLocalX(float x) {
        base.SetLocalX(x);
        if (!isPortalActived) return;
        //transform.position = new Vector3(x, transform.position.y, transform.position.z);
        if (player.positionId.y == id.y && player.positionId.z == id.z) {
            gameSystemMgr.isCleared = true;
            gameSystemMgr.m_nextStageId = targetStage;
            stageMgr.SetPlayerId(targetTileId);
            
        }
    }

    public override void SetLocalZ(float z) {
        base.SetLocalZ(z);
        if (!isPortalActived) return;
        //transform.position = new Vector3(transform.position.x, transform.position.y, z);
        if (player.positionId.y == id.y && player.positionId.x == id.x) {
            gameSystemMgr.isCleared = true;
            gameSystemMgr.m_nextStageId = targetStage;
            stageMgr.SetPlayerId(targetTileId);
        }
    }

    public void SetTargetStage(int stage) {
        targetStage = stage;
    }

    public void SetTargetTileId(Vector3 id) {
        targetTileId = id;
    }
}
