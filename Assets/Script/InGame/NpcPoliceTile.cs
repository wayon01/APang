﻿using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class NpcPoliceTile : AttackTile {

    private APlayer player;
    private GameSystemMgr gameSystemMgr;

    // Use this for initialization
    void Start () {
        player = GameObject.Find("player").GetComponent<APlayer>();
        gameSystemMgr = GameObject.Find("GameSystemManager").GetComponent<GameSystemMgr>();
        _bIsCanIgnoreBlock = true;
    }
	
	// Update is called once per frame
	void Update () {
        if (!Input.GetMouseButtonDown(0) && !Input.GetMouseButton(0))
            transform.rotation = Quaternion.Lerp(transform.rotation, Camera.main.transform.rotation, 0.1f);

    }

    public override void SetLocalX(float x) {
        base.SetLocalX(x);
        //transform.position = new Vector3(x, transform.position.y, transform.position.z);
        if (player.positionId.y + 1 == id.y && player.positionId.z == id.z) {
            gameSystemMgr.isFailed = true;
        }
    }

    public override void SetLocalZ(float z) {
        base.SetLocalZ(z);
        //transform.position = new Vector3(transform.position.x, transform.position.y, z);
        if (player.positionId.y + 1 == id.y && player.positionId.x == id.x) {
            gameSystemMgr.isFailed = true;
        }
    }

    public new void SetRigid(bool activated) {
        base.SetRigid(activated);
    }

    public override void Init() {
        base.Init();

    }
}
