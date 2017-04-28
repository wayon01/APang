using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class PortalTile : TileObject {
    
    private APlayer player;
    private GameSystemMgr gameSystemMgr;
    private StageMgr stageMgr;
    private CameraMgr cameraMgr;

    public Vector3 targetTileId;
    public int targetStage;
    public Color portalColor;

    private bool isPortalActived;
    private ParticleSystem particleSettings;

    void Awake() {
        targetTileId = -Vector3.one;
        targetStage = -2;
        isPortalActived = false;
    }

    // Use this for initialization
    void Start () {
        player = GameObject.Find("player").GetComponent<APlayer>();
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        stageMgr = GameObject.Find("StageManager").GetComponent<StageMgr>();
        cameraMgr = GameObject.Find("CameraManager").GetComponent<CameraMgr>();
        particleSettings = GetComponent<ParticleSystem>();


    }
	
	// Update is called once per frame
	void Update () {
	    if (!isPortalActived && gameSystemMgr.isPlayerMovingUp) {
	        isPortalActived = true;
	    }

	    if (!cameraMgr.isRelax && Input.GetMouseButtonUp(0)) {
	        var main = particleSettings.main;
            main.startColor = new Color(portalColor.r / 4, portalColor.g / 4, portalColor.b / 4, 0.2f);
        }
	}

    public override void SetLocalX(float x) {
        base.SetLocalX(x);
        var main = particleSettings.main;
        main.startColor = portalColor;
        if (!isPortalActived) return;
        //transform.position = new Vector3(x, transform.position.y, transform.position.z);
        if (player.positionId.y == id.y && player.positionId.z == id.z) {
            if (targetStage == -2) return;
            gameSystemMgr.isCleared = true;
            gameSystemMgr.m_nextStageId = targetStage;
            stageMgr.SetPlayerId(targetTileId);
            
        }
    }

    public override void SetLocalZ(float z) {
        base.SetLocalZ(z);
        var main = particleSettings.main;
        main.startColor = portalColor;
        if (!isPortalActived) return;
        //transform.position = new Vector3(transform.position.x, transform.position.y, z);
        if (player.positionId.y == id.y && player.positionId.x == id.x) {
            if (targetStage == -2) return;
            gameSystemMgr.isCleared = true;
            gameSystemMgr.m_nextStageId = targetStage;
            stageMgr.SetPlayerId(targetTileId);
        }
    }

    public void SetTargetStage(int stage) {
        targetStage = stage;
    }

    public void SetTargetTileId(Vector3 id, Color color) {
        targetTileId = id;
    }

    public void SetPortalColor(Color color) {
        portalColor = new Color(color.r, color.g, color.b, color.a);
        var main = GetComponent<ParticleSystem>().main;
        main.startColor = portalColor;

        Debug.Log(name + " Color : " + color.r + ", " + color.g + ", " + color.b);
    }
}
