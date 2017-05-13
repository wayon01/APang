using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class SwitchTile : TileObject {

    private APlayer player;
    private GameSystemMgr gameSystemMgr;

    private TweenScale tweenScale;
    private bool isUsed;
    private Collider collider;

    // Use this for initialization
    void Start () {
        _bIsCanIgnoreBlock = true;
        isUsed = false;

        player = GameObject.Find("player").GetComponent<APlayer>();
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        tweenScale = gameObject.GetComponent<TweenScale>();
        collider = GetComponent<SphereCollider>();
        collider.enabled = false;
    }
	
	// Update is called once per frame
	void Update () {
	    if (gameSystemMgr.isFailed && !collider.enabled) {
            collider.enabled = true;
        }
	}

    public override void Init() {
        base.Init();
    }

    public override void SetLocalX(float x) {
        base.SetLocalX(x);
        if (player.positionId.y + 1 == id.y && player.positionId.z == id.z) {
            if (!isUsed) {
                isUsed = true;
                tweenScale.enabled = true;
                gameSystemMgr.m_playerMovingCount++;
            }
        }
    }

    public override void SetLocalZ(float z) {
        base.SetLocalZ(z);
        if (player.positionId.y + 1 == id.y && player.positionId.x == id.x) {
            if (!isUsed) {
                isUsed = true;
                tweenScale.enabled = true;
                gameSystemMgr.m_playerMovingCount++;
            }
        }
    }
}
